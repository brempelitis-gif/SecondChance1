
#pragma once

#include "CoreMinimal.h"
#include "UI/Base/UIBaseWidget.h"
#include "Core/Enums/ESettingsCategory.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "UI/Settings/AudioOptionType.h"
#include "OptionsBaseWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsChanged, ESettingsCategory, Category);

UCLASS()
class SECONDCHANCE_API UOptionsBaseWidget : public UUIBaseWidget
{
    GENERATED_BODY()


public:
	//virtual void NativeOnInitialized() override;
	//virtual void NativeDestruct() override;
	
    UPROPERTY(BlueprintAssignable, Category = "Settings")
    FOnSettingsChanged OnSettingsChanged;

    /** Audio Iestatījumi */
    UFUNCTION(BlueprintCallable, Category="Settings|Audio")
    void SetAudioOption(EAudioOption Option, float Value);

    UFUNCTION(BlueprintCallable, Category="Settings|Audio")
    void ApplyAudioSettings();

    UFUNCTION(BlueprintCallable, Category="Settings|Audio")
    void CancelAudioSettings();

    /** Grafikas Iestatījumi */
    UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
    void ApplyGraphicsSettings();

    UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
    void CancelGraphicsSettings();

    /** Pending sistēma */
    UFUNCTION(BlueprintPure, Category="UI|Options")
    bool IsCategoryPending(ESettingsCategory Category) const;

    UFUNCTION(BlueprintCallable, Category="Settings")
    void ApplyPendingSettings();

    UFUNCTION(BlueprintCallable, Category="Settings")
    void CancelPendingSettings();

    // Getter vērtības priekš Refresh funkcijām kategorijās
    float GetPendingMasterVolume() const { return PendingMasterVolume; }
    float GetPendingMusicVolume() const { return PendingMusicVolume; }
    float GetPendingSFXVolume() const { return PendingSFXVolume; }

protected:
    virtual void NativeConstruct() override;

	static inline const FString AudioSettingsSlot = TEXT("AudioSettings");
	static constexpr int32 AudioSettingsUserIndex = 0;
	
    void LoadAudioSettings();
    void SaveAudioSettings() const;
    void SetMasterVolume(float Value) const;
    void SetMusicVolume(float Value) const;
    void SetSFXVolume(float Value) const;
public:
    void MarkCategoryPending(ESettingsCategory Category);
    void ClearCategoryPending(ESettingsCategory Category);

private:
    float CurrentMasterVolume = 1.f;
    float CurrentMusicVolume = 1.f;
    float CurrentSFXVolume = 1.f;

    float PendingMasterVolume = 1.f;
    float PendingMusicVolume = 1.f;
    float PendingSFXVolume = 1.f;

    UPROPERTY()
    TSet<ESettingsCategory> PendingCategories;

//man sis jau bija

protected:

	// =========================
	// UI Manager
	// =========================
	UPROPERTY()
	UUIManagerSubsystem* UIManager = nullptr;

	// =========================
	// Category
	// =========================
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Options")
	ESettingsCategory Category = ESettingsCategory::None;

	// =========================
	// Buttons (common)
	// =========================
	UPROPERTY(meta = (BindWidgetOptional))
	class UMenuButtonWidget* ApplyButton;

	UPROPERTY(meta = (BindWidgetOptional))
	class UMenuButtonWidget* CancelButton;

	// =========================
	// Button handlers
	// =========================
//	UFUNCTION()
//	void HandleApplyClicked();

//	UFUNCTION()
//	void HandleCancelClicked();
};
