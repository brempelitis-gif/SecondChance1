#include "UIOptionsMenuBase.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Sound/SoundClass.h"
#include "Core/Save/AudioSettingsSaveGame.h"
#include "Core/Subsystems/UIConfig.h"

void UUIOptionsMenuBase::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BindButtons();
}

void UUIOptionsMenuBase::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (AudioTab) AudioTab->SetLabel(AudioTabLabel);
    if (GraphicsTab) GraphicsTab->SetLabel(GraphicsTabLabel);
    if (ControlsTab) ControlsTab->SetLabel(ControlsTabLabel);
    if (GameplayTab) GameplayTab->SetLabel(GameplayTabLabel);
    if (ApplyButton) ApplyButton->SetLabel(ApplyButtonLabel);
    if (CancelButton) CancelButton->SetLabel(CancelButtonLabel);
}

void UUIOptionsMenuBase::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 1. Ielādējam iestatījumus no faila
    LoadAudioSettings();
    
    // 2. UZREIZ IESTATĀM AUDIO KATEGORIJU KĀ AKTĪVO
    SetActiveCategory(ESettingsCategory::Audio);
    
    // 3. Paslēpjam Apply/Cancel, jo sākumā izmaiņu nav
    UpdateActionButtonsVisibility();
}

void UUIOptionsMenuBase::BindButtons()
{
    // Svarīgi: Pārliecinies, ka tavā MenuButtonWidget ir OnButtonClicked delegāts (Dynamic)
    if (AudioTab) AudioTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleAudioTab);
    if (GraphicsTab) GraphicsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGraphicsTab);
    if (ControlsTab) ControlsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleControlsTab);
    if (GameplayTab) GameplayTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGameplayTab);

    if (ApplyButton) ApplyButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleApply);
    if (CancelButton) CancelButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleCancel);
}

/* === SETTINGS LOGIC === */

void UUIOptionsMenuBase::SetAudioOption(EAudioOption Option, float Value)
{
    switch (Option)
    {
        case EAudioOption::Master: PendingMasterVolume = Value; SetMasterVolume(Value); break;
        case EAudioOption::Music: PendingMusicVolume = Value; SetMusicVolume(Value); break;
        case EAudioOption::SFX: PendingSFXVolume = Value; SetSFXVolume(Value); break;
    }
    MarkCategoryPending(ESettingsCategory::Audio);
}

void UUIOptionsMenuBase::MarkCategoryPending(ESettingsCategory Category)
{
    if (Category != ESettingsCategory::None)
    {
        PendingCategories.Add(Category);
        OnSettingsChanged.Broadcast(Category);
        UpdateActionButtonsVisibility();
    }
}

void UUIOptionsMenuBase::ApplyAudioSettings()
{
    CurrentMasterVolume = PendingMasterVolume;
    CurrentMusicVolume = PendingMusicVolume;
    CurrentSFXVolume = PendingSFXVolume;
    SaveAudioSettings();
}

void UUIOptionsMenuBase::CancelAudioSettings()
{
    PendingMasterVolume = CurrentMasterVolume;
    PendingMusicVolume = CurrentMusicVolume;
    PendingSFXVolume = CurrentSFXVolume;
    SetMasterVolume(CurrentMasterVolume);
    SetMusicVolume(CurrentMusicVolume);
    SetSFXVolume(CurrentSFXVolume);
}

void UUIOptionsMenuBase::ApplyGraphicsSettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->ApplySettings(true);
        Settings->ConfirmVideoMode();
        Settings->SaveSettings();
    }
}

void UUIOptionsMenuBase::CancelGraphicsSettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->LoadSettings(true);
        Settings->ApplySettings(false);
    }
}

/* === HANDLERS === */

void UUIOptionsMenuBase::HandleApply()
{
    if (PendingCategories.Contains(ESettingsCategory::Audio)) ApplyAudioSettings();
    if (PendingCategories.Contains(ESettingsCategory::Graphics)) ApplyGraphicsSettings();
    
    PendingCategories.Empty();
    OnSettingsChanged.Broadcast(ESettingsCategory::None); 
    UpdateActionButtonsVisibility();
}

void UUIOptionsMenuBase::HandleCancel()
{
    if (PendingCategories.Contains(ESettingsCategory::Audio)) CancelAudioSettings();
    if (PendingCategories.Contains(ESettingsCategory::Graphics)) CancelGraphicsSettings();

    PendingCategories.Empty();
    OnSettingsChanged.Broadcast(ESettingsCategory::None);
    UpdateActionButtonsVisibility();


}

void UUIOptionsMenuBase::UpdateActionButtonsVisibility()
{
    bool bHasChanges = PendingCategories.Num() > 0;
    ESlateVisibility NewVis = bHasChanges ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
    
    if (ApplyButton) ApplyButton->SetVisibility(NewVis);
    if (CancelButton) CancelButton->SetVisibility(NewVis);
}

void UUIOptionsMenuBase::SetActiveCategory(ESettingsCategory Category)
{
    CurrentCategory = Category;
    if (CategorySwitcher)
    {
        int32 Index = 0;
        switch(Category) {
            case ESettingsCategory::Audio:    Index = 0; break;
            case ESettingsCategory::Graphics: Index = 1; break;
            case ESettingsCategory::Controls: Index = 2; break;
            case ESettingsCategory::Gameplay: Index = 3; break;
            default: Index = 0;
        }
        CategorySwitcher->SetActiveWidgetIndex(Index);
    }
}

void UUIOptionsMenuBase::HandleAudioTab() { SetActiveCategory(ESettingsCategory::Audio); }
void UUIOptionsMenuBase::HandleGraphicsTab() { SetActiveCategory(ESettingsCategory::Graphics); }
void UUIOptionsMenuBase::HandleControlsTab() { SetActiveCategory(ESettingsCategory::Controls); }
void UUIOptionsMenuBase::HandleGameplayTab() { SetActiveCategory(ESettingsCategory::Gameplay); }

void UUIOptionsMenuBase::HandleBack()
{
     // Noņemam šo logrīku no ekrāna caur UIManager
    UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan)
    {
        UIMan->PopWidget(); // Šis noņems Options un atgriezīs fokusu uz Main Menu
    }
}

/* === AUDIO INTERNALS === */

void UUIOptionsMenuBase::SetMasterVolume(float Value) const
{
    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->MasterSoundClass)
    {
        UIMan->UIConfig->MasterSoundClass->Properties.Volume = Value;
    }
}

void UUIOptionsMenuBase::SetMusicVolume(float Value) const
{
    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->MusicSoundClass)
    {
        UIMan->UIConfig->MusicSoundClass->Properties.Volume = Value;
    }
}

void UUIOptionsMenuBase::SetSFXVolume(float Value) const
{
    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->SFXSoundClass)
    {
        UIMan->UIConfig->SFXSoundClass->Properties.Volume = Value;
    }
}

void UUIOptionsMenuBase::LoadAudioSettings()
{
    if (!UGameplayStatics::DoesSaveGameExist(AudioSettingsSlot, AudioSettingsUserIndex)) return;
    
    UAudioSettingsSaveGame* Save = Cast<UAudioSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(AudioSettingsSlot, AudioSettingsUserIndex));
    if (Save)
    {
        CurrentMasterVolume = PendingMasterVolume = Save->MasterVolume;
        CurrentMusicVolume = PendingMusicVolume = Save->MusicVolume;
        CurrentSFXVolume = PendingSFXVolume = Save->SFXVolume;
        
        SetMasterVolume(CurrentMasterVolume); 
        SetMusicVolume(CurrentMusicVolume); 
        SetSFXVolume(CurrentSFXVolume);
    }
}

void UUIOptionsMenuBase::SaveAudioSettings() const
{
    UAudioSettingsSaveGame* Save = Cast<UAudioSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass()));
    if (Save)
    {
        Save->MasterVolume = CurrentMasterVolume; 
        Save->MusicVolume = CurrentMusicVolume; 
        Save->SFXVolume = CurrentSFXVolume;
        UGameplayStatics::SaveGameToSlot(Save, AudioSettingsSlot, AudioSettingsUserIndex);
    }
}

bool UUIOptionsMenuBase::IsCategoryPending(ESettingsCategory Category) const 
{ 
    return PendingCategories.Contains(Category); 
}