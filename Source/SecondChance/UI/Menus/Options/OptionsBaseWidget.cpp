#include "UI/Menus/Options/OptionsBaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Core/Save/AudioSettingsSaveGame.h"
#include "Sound/SoundClass.h"

//static const FString AudioSettingsSlot = TEXT("AudioSettings");
//static constexpr int32 AudioSettingsUserIndex = 0;

void UOptionsBaseWidget::NativeConstruct()
{
    Super::NativeConstruct();
    LoadAudioSettings();
}

// ================= AUDIO =================
void UOptionsBaseWidget::SetAudioOption(EAudioOption Option, float Value)
{
    switch (Option)
    {
        case EAudioOption::Master: PendingMasterVolume = Value; SetMasterVolume(Value); break;
        case EAudioOption::Music: PendingMusicVolume = Value; SetMusicVolume(Value); break;
        case EAudioOption::SFX: PendingSFXVolume = Value; SetSFXVolume(Value); break;
    }
    MarkCategoryPending(ESettingsCategory::Audio);
}

void UOptionsBaseWidget::SetMasterVolume(float Value) const
{
    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->MasterSoundClass)
        UIMan->UIConfig->MasterSoundClass->Properties.Volume = Value;
}

void UOptionsBaseWidget::SetMusicVolume(float Value) const
{
    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->MusicSoundClass)
        UIMan->UIConfig->MusicSoundClass->Properties.Volume = Value;
}

void UOptionsBaseWidget::SetSFXVolume(float Value) const
{
    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->SFXSoundClass)
        UIMan->UIConfig->SFXSoundClass->Properties.Volume = Value;
}

void UOptionsBaseWidget::ApplyAudioSettings()
{
    CurrentMasterVolume = PendingMasterVolume;
    CurrentMusicVolume = PendingMusicVolume;
    CurrentSFXVolume = PendingSFXVolume;
    SaveAudioSettings();
    ClearCategoryPending(ESettingsCategory::Audio);
}

void UOptionsBaseWidget::CancelAudioSettings()
{
    PendingMasterVolume = CurrentMasterVolume;
    PendingMusicVolume = CurrentMusicVolume;
    PendingSFXVolume = CurrentSFXVolume;
    SetMasterVolume(CurrentMasterVolume);
    SetMusicVolume(CurrentMusicVolume);
    SetSFXVolume(CurrentSFXVolume);
    ClearCategoryPending(ESettingsCategory::Audio);
}

void UOptionsBaseWidget::LoadAudioSettings()
{
    if (!UGameplayStatics::DoesSaveGameExist(AudioSettingsSlot, AudioSettingsUserIndex)) return;
    UAudioSettingsSaveGame* Save = Cast<UAudioSettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(AudioSettingsSlot, AudioSettingsUserIndex));
    if (Save)
    {
        CurrentMasterVolume = PendingMasterVolume = Save->MasterVolume;
        CurrentMusicVolume = PendingMusicVolume = Save->MusicVolume;
        CurrentSFXVolume = PendingSFXVolume = Save->SFXVolume;
        SetMasterVolume(CurrentMasterVolume); SetMusicVolume(CurrentMusicVolume); SetSFXVolume(CurrentSFXVolume);
    }
}

void UOptionsBaseWidget::SaveAudioSettings() const
{
    UAudioSettingsSaveGame* Save = Cast<UAudioSettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass()));
    if (Save)
    {
        Save->MasterVolume = CurrentMasterVolume; Save->MusicVolume = CurrentMusicVolume; Save->SFXVolume = CurrentSFXVolume;
        UGameplayStatics::SaveGameToSlot(Save, AudioSettingsSlot, AudioSettingsUserIndex);
    }
}

// ================= GRAPHICS =================
void UOptionsBaseWidget::ApplyGraphicsSettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->ApplySettings(true);
        Settings->ConfirmVideoMode();
        Settings->SaveSettings();
        ClearCategoryPending(ESettingsCategory::Graphics);
    }
}

void UOptionsBaseWidget::CancelGraphicsSettings()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->LoadSettings(true);
        Settings->ApplySettings(false);
        ClearCategoryPending(ESettingsCategory::Graphics);
    }
}

// ================= PENDING =================
void UOptionsBaseWidget::MarkCategoryPending(ESettingsCategory Cat)
{
    if (Cat != ESettingsCategory::None && !PendingCategories.Contains(Cat))
    {
        PendingCategories.Add(Cat);
        OnSettingsChanged.Broadcast(Cat);
    }
}

void UOptionsBaseWidget::ClearCategoryPending(ESettingsCategory Cat)
{
    if (PendingCategories.Remove(Cat) > 0) OnSettingsChanged.Broadcast(Cat);
}

bool UOptionsBaseWidget::IsCategoryPending(ESettingsCategory Cat) const { return PendingCategories.Contains(Cat); }

void UOptionsBaseWidget::ApplyPendingSettings()
{
    if (PendingCategories.Contains(ESettingsCategory::Audio)) ApplyAudioSettings();
    if (PendingCategories.Contains(ESettingsCategory::Graphics)) ApplyGraphicsSettings();
    PendingCategories.Empty();
    OnSettingsChanged.Broadcast(ESettingsCategory::None);
}

void UOptionsBaseWidget::CancelPendingSettings()
{
    if (PendingCategories.Contains(ESettingsCategory::Audio)) CancelAudioSettings();
    if (PendingCategories.Contains(ESettingsCategory::Graphics)) CancelGraphicsSettings();
    PendingCategories.Empty();
    OnSettingsChanged.Broadcast(ESettingsCategory::None);
}