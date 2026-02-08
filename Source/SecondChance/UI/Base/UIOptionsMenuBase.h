#pragma once

#include "CoreMinimal.h"
#include "Core/Enums/ESettingsCategory.h"
#include "UI/Base/UIBaseWidget.h"
#include "UI/Settings/AudioOptionType.h"
#include "UIOptionsMenuBase.generated.h"

class UMenuButtonWidget;
class UWidgetSwitcher;
class UUIManagerSubsystem;
class UUIConfirmationPopup;

UCLASS()
class SECONDCHANCE_API UUIOptionsMenuBase : public UUIBaseWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Settings")
    FOnSettingsChanged OnSettingsChanged;

protected:
    virtual void NativeOnInitialized() override;
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    /* =======================
     * IEKŠĒJĀS KONSTANTES (Izlabo C2374)
     * ======================= */
    static inline const FString AudioSettingsSlot = TEXT("AudioSettings");
    static constexpr int32 AudioSettingsUserIndex = 0;

    /* =======================
     * TOP BUTTONS (Tabs)
     * ======================= */
    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* AudioTab;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText AudioTabLabel = FText::FromString("Audio");

    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* GraphicsTab;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText GraphicsTabLabel = FText::FromString("Graphics");

    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* ControlsTab;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText ControlsTabLabel = FText::FromString("Controls");

    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* GameplayTab;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText GameplayTabLabel = FText::FromString("Gameplay");

    
    /* =======================
     * POPUP & SWITCHER
     * ======================= */
    UPROPERTY(EditAnywhere, Category = "UI|Settings")
    TSubclassOf<UUIConfirmationPopup> PopupClass;

    UPROPERTY()
    UUIConfirmationPopup* CachedPopup;

    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* CategorySwitcher;

    /* =======================
     * BOTTOM ACTIONS
     * ======================= */
    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* BackButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText BackButtonLabel = FText::FromString("Back");
    
    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* ApplyButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText ApplyButtonLabel = FText::FromString("Apply");

    UPROPERTY(meta = (BindWidget))
    UMenuButtonWidget* CancelButton;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText CancelButtonLabel = FText::FromString("Cancel");

public:
    void SetAudioOption(EAudioOption Option, float Value);
    bool IsCategoryPending(ESettingsCategory Category) const;
    void MarkCategoryPending(ESettingsCategory Category);
    
    float GetPendingMasterVolume() const { return PendingMasterVolume; }
    float GetPendingMusicVolume() const { return PendingMusicVolume; }
    float GetPendingSFXVolume() const { return PendingSFXVolume; }

private:
    void BindButtons();
    void SetActiveCategory(ESettingsCategory Category);
    
    UFUNCTION() void HandleAudioTab();
    UFUNCTION() void HandleGraphicsTab();
    UFUNCTION() void HandleControlsTab();
    UFUNCTION() void HandleGameplayTab();

    UFUNCTION() void HandleBack();
    UFUNCTION() void HandleApply();
    UFUNCTION() void HandleCancel();
    
    void ApplyAudioSettings();
    void CancelAudioSettings();
    void ApplyGraphicsSettings();
    UFUNCTION() void ConfirmGraphicsChanges();
    UFUNCTION() void RevertGraphicsChanges();
    void CancelGraphicsSettings();

    void LoadAudioSettings();
    void SaveAudioSettings() const;
    void SetMasterVolume(float Value) const;
    void SetMusicVolume(float Value) const;
    void SetSFXVolume(float Value) const;

    ESettingsCategory CurrentCategory = ESettingsCategory::Audio;
    
    UPROPERTY()
    TSet<ESettingsCategory> PendingCategories;

    float CurrentMasterVolume = 1.f;
    float CurrentMusicVolume = 1.f;
    float CurrentSFXVolume = 1.f;
    float PendingMasterVolume = 1.f;
    float PendingMusicVolume = 1.f;
    float PendingSFXVolume = 1.f;

protected:
    UFUNCTION(BlueprintCallable, Category="Options")
    virtual void UpdateActionButtonsVisibility();
};