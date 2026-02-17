#include "UI/Menus/Options/OptionsBaseWidget.h"

#include "Core/Enums/ESettingsCategory.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Core/Subsystems/AudioManagerSubsystem.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "UI/Base/ConfirmationPopUp/UIConfirmationPopup.h" // Pieņemot, ka tev ir šis fails
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void UOptionsBaseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // 1. Kešojam Subsystemus (Optimization)
    if (UGameInstance* GI = GetGameInstance())
    {
        UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
        AudioManager = GI->GetSubsystem<UAudioManagerSubsystem>();
    }
}

void UOptionsBaseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 2. Iegūstam aktuālos datus un atjaunojam UI (Slaiderus)
    if (AudioManager)
    {
        // Šis izsauks BP eventu un noliks slaiderus pareizajās vietās
        UpdateAudioSliders(AudioManager->GetCurrentAudioSettings());
    }

    // Paslēpjam pogas, jo sākumā nav izmaiņu
    PendingCategories.Empty();
    UpdateActionButtonsVisibility();
}

// ================= AUDIO LOGIC =================

void UOptionsBaseWidget::SetAudioOption(EAudioOption Option, float Value)
{
    if (AudioManager)
    {
        // Nosūtām jauno vērtību uz Subsystemu (Live Preview)
        AudioManager->SetPendingVolume(Option, Value);
        
        // Atzīmējam, ka mums ir nesaglabātas izmaiņas
        MarkCategoryPending(ESettingsCategory::Audio);
    }
}

void UOptionsBaseWidget::ApplyAudioChanges()
{
    if (AudioManager)
    {
        AudioManager->ApplySettings(); // Ieraksta failā
        ClearCategoryPending(ESettingsCategory::Audio);
    }
}

void UOptionsBaseWidget::CancelAudioChanges()
{
    if (AudioManager)
    {
        AudioManager->CancelSettings(); // Atgriež skaņu uz veco
        
        // Svarīgi: Liekam UI slaideriem aizlekt atpakaļ
        UpdateAudioSliders(AudioManager->GetCurrentAudioSettings());
        
        ClearCategoryPending(ESettingsCategory::Audio);
    }
}

// ================= GRAPHICS LOGIC =================

void UOptionsBaseWidget::ApplyGraphicsResolution(FIntPoint Resolution)
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->SetScreenResolution(Resolution);
        MarkCategoryPending(ESettingsCategory::Graphics);
    }
}

void UOptionsBaseWidget::ApplyGraphicsChanges()
{
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (!Settings || !UIManager) return;

    // 1. Pielietojam iestatījumus (bet vēl nesaglabājam failā)
    Settings->ApplySettings(false);

    // 2. Parādām Pop-up logu (lai apstiprinātu)
    UUIConfirmationPopup* Popup = UIManager->PushConfirmationPopup(
        FText::FromString("Keep these settings?"), 
        15.0f // 15 sekundes taimeris
    );

    if (Popup)
    {
        Popup->OnConfirmed.AddUniqueDynamic(this, &UOptionsBaseWidget::OnGraphicsConfirmed);
        Popup->OnTimedOutOrCancelled.AddUniqueDynamic(this, &UOptionsBaseWidget::OnGraphicsReverted);
        
        // Aizveram popupu pēc izvēles
        Popup->OnConfirmed.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::PopWidget);
        Popup->OnTimedOutOrCancelled.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::PopWidget);
    }
}

void UOptionsBaseWidget::OnGraphicsConfirmed()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        Settings->ConfirmVideoMode();
        Settings->SaveSettings(); // Tikai tagad rakstām failā
        ClearCategoryPending(ESettingsCategory::Graphics);
    }
}

void UOptionsBaseWidget::OnGraphicsReverted()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        // Ielādējam vecos iestatījumus no faila
        Settings->LoadSettings(true); 
        
        // Piespiedu kārtā atjaunojam rezolūciju (jo LoadSettings reizēm negrib pārrakstīt dirty flag)
        Settings->SetScreenResolution(Settings->GetLastConfirmedScreenResolution());
        Settings->SetFullscreenMode(Settings->GetLastConfirmedFullscreenMode());
        
        Settings->ApplySettings(false);

        ClearCategoryPending(ESettingsCategory::Graphics);
        
        // Šeit vajadzētu arī BP eventu, lai atjaunotu Dropdownus (līdzīgi kā audio)
        // UpdateGraphicsDropdowns(...); 
    }
}

void UOptionsBaseWidget::CancelGraphicsChanges()
{
    // Tas pats, kas Revert, bet bez popupa
    OnGraphicsReverted(); 
}

// ================= GENERAL BUTTON HANDLERS =================

void UOptionsBaseWidget::HandleApplyClicked()
{
    if (PendingCategories.Contains(ESettingsCategory::Audio))
    {
        ApplyAudioChanges();
    }
    
    if (PendingCategories.Contains(ESettingsCategory::Graphics))
    {
        ApplyGraphicsChanges();
    }
    
    // Graphics kategoriju neizņemam šeit, to izdarīs OnGraphicsConfirmed
}

void UOptionsBaseWidget::HandleCancelClicked()
{
    if (PendingCategories.Contains(ESettingsCategory::Audio)) CancelAudioChanges();
    if (PendingCategories.Contains(ESettingsCategory::Graphics)) CancelGraphicsChanges();
    
    PendingCategories.Empty();
    UpdateActionButtonsVisibility();
}

void UOptionsBaseWidget::HandleBackClicked()
{
    // Ja ir izmaiņas -> Brīdinām
    if (PendingCategories.Num() > 0 && UIManager)
    {
        UUIConfirmationPopup* Popup = UIManager->PushConfirmationPopup(FText::FromString("Unsaved Changes! Discard?"), 0.0f);
        if (Popup)
        {
            // Ja saka "Jā" (Discard) -> Cancel changes un ejam ārā
            Popup->OnConfirmed.AddUniqueDynamic(this, &UOptionsBaseWidget::HandleCancelClicked);
            Popup->OnConfirmed.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::PopWidget); // Close Popup
            Popup->OnConfirmed.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::PopWidget); // Close Options
            
            Popup->OnTimedOutOrCancelled.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::PopWidget); // Just Close Popup
        }
    }
    // Ja nav izmaiņu -> Ejam ārā uzreiz
    else if (UIManager)
    {
        UIManager->PopWidget();
    }
}

bool UOptionsBaseWidget::IsCategoryPending(ESettingsCategory Category) const
{
    return PendingCategories.Contains(Category);
}

// ================= HELPER FUNCTIONS =================

void UOptionsBaseWidget::MarkCategoryPending(ESettingsCategory Category)
{
    if (Category != ESettingsCategory::None)
    {
        PendingCategories.Add(Category);
        UpdateActionButtonsVisibility();
    }
}

void UOptionsBaseWidget::ClearCategoryPending(ESettingsCategory Category)
{
    PendingCategories.Remove(Category);
    UpdateActionButtonsVisibility();
}

void UOptionsBaseWidget::UpdateActionButtonsVisibility()
{
    bool bHasChanges = PendingCategories.Num() > 0;
    ESlateVisibility NewVis = bHasChanges ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;

    if (ApplyButton) ApplyButton->SetVisibility(NewVis);
    if (CancelButton) CancelButton->SetVisibility(NewVis);
}
