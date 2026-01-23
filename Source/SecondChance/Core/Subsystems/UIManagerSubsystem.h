#pragma once

#include "UIActions.h"
#include "UIConfig.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

class UUIBaseWidget;


UENUM(BlueprintType)
enum class ESettingsCategory : uint8
{
	Audio,
	Graphics,
	Controls,
	Gameplay,
	None
};
/* =====================================================
 * UI STATE
 * ===================================================== */

UENUM(BlueprintType)
enum class EUIState : uint8
{
	None,
	Splash,
	MainMenu,
	Options,
	Pause
};

/* =====================================================
 * UI MANAGER SUBSYSTEM
 * ===================================================== */

UCLASS(BlueprintType)
class SECONDCHANCE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/* =====================================================
	 * CORE
	 * ===================================================== */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	UUIConfig* UIConfig;

	UPROPERTY()
	TArray<FIntPoint> SupportedResolutions;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/* =====================================================
	 * UI FLOW
	 * ===================================================== */

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowSplashScreen();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowOptionsMenu();

	UFUNCTION(BlueprintCallable, Category="UI")
	void ShowPauseMenu();

	UFUNCTION(BlueprintCallable, Category="UI")
	void HideCurrentUI();

	UFUNCTION(BlueprintCallable, Category="UI")
	void HandleMenuAction(EUIAction Action);

	EUIState GetCurrentState() const;
	
	


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
		FOnConfirmSettingsRequired,
		ESettingsCategory,
		ChangeType
	);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
		FOnSettingsChanged,
		ESettingsCategory,
		Category
	);

	UPROPERTY(BlueprintAssignable)
	FOnSettingsChanged OnSettingsChanged;

	UPROPERTY(BlueprintAssignable, Category="Settings")
	FOnConfirmSettingsRequired OnConfirmSettingsRequired;
	// ============================================================
	// NEW GAME
	// ============================================================
	void HandleNewGame();
	// ============================================================
	// Load GAME
	// ============================================================
	void HandleLoadGame() const;
	/* =====================================================
	 * AUDIO SETTINGS
	 * ===================================================== */


	// Set / Apply / Cancel
	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	void SetAudioOption(EAudioOption Option, float Value);

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	void ApplyAudioSettings();

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	void CancelAudioSettings();

	// Getters (UI)
	UFUNCTION(BlueprintPure, Category="UI|Options")
	bool IsCategoryPending(ESettingsCategory Category) const;
	
	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	float GetMasterVolume() const { return CurrentMasterVolume; }

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	float GetMusicVolume() const { return CurrentMusicVolume; }

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	float GetSFXVolume() const { return CurrentSFXVolume; }

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	float GetPendingMasterVolume() const { return PendingMasterVolume; }

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	float GetPendingMusicVolume() const { return PendingMusicVolume; }

	UFUNCTION(BlueprintCallable, Category="Settings|Audio")
	float GetPendingSFXVolume() const { return PendingSFXVolume; }

	void SaveAudioSettings() const;
	void LoadAudioSettings();
	
	/* =====================================================
	 * GRAPHICS SETTINGS
	 * ===================================================== */
	
	UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
	void SetResolution(int32 Index);

	UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
	void SetWindowMode(int32 Index);

	UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
	void SetGraphicsQuality(int32 Index);

	UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
	void ApplyGraphicsSettings();

	UFUNCTION(BlueprintCallable, Category="Settings|Graphics")
	void CancelGraphicsSettings();

	UFUNCTION(BlueprintCallable, Category="UI|Graphics")
	int32 GetPendingResolutionIndex() const { return PendingResolutionIndex; }

	UFUNCTION(BlueprintCallable, Category="UI|Graphics")
	int32 GetPendingWindowMode() const { return PendingWindowMode; }

	UFUNCTION(BlueprintCallable, Category="UI|Graphics")
	int32 GetPendingQuality() const { return PendingQuality; }
	// Internal audio apply
	void SetMasterVolume(float Value) const;
	void SetMusicVolume(float Value) const;
	void SetSFXVolume(float Value) const;
	/* =====================================================
	 * FUTURE: CONTROLS SETTINGS
	 * ===================================================== */
	// TODO: Input rebinding
	// Pending / Apply / Cancel
	UFUNCTION(BlueprintCallable, Category="Settings")
	void CancelPendingSettings();
	UFUNCTION(BlueprintCallable, Category="Settings")
	void ApplyPendingSettings();
	UFUNCTION(BlueprintCallable, Category="Settings")
	void ApplyPendingSettings_Internal();
	/* =====================================================
	 * FUTURE: ACCESSIBILITY SETTINGS
	 * ===================================================== */
	// TODO: UI Scale, Colorblind, Subtitles

private:
	/* =====================================================
	 * INTERNAL UI STATE
	 * ===================================================== */
	
	void SetUIState(EUIState NewState);
	void CreateAndShowWidget(TSubclassOf<UUIBaseWidget> WidgetClass);

	UPROPERTY()
	UUIBaseWidget* CurrentWidget = nullptr;

	UPROPERTY()
	EUIState CurrentState = EUIState::None;

	/* =====================================================
	 * AUDIO STATE
	 * ===================================================== */

	float CurrentMasterVolume = 1.f;
	float CurrentMusicVolume  = 1.f;
	float CurrentSFXVolume    = 1.f;

	float PendingMasterVolume = 1.f;
	float PendingMusicVolume  = 1.f;
	float PendingSFXVolume    = 1.f;

	/* =====================================================
	 * GRAPHICS STATE
	 * ===================================================== */

	int32 CurrentResolutionIndex = 0;
	int32 CurrentWindowMode      = 0;
	int32 CurrentQuality         = 2;

	int32 PendingResolutionIndex = 0;
	int32 PendingWindowMode      = 0;
	int32 PendingQuality         = 2;

	
	UPROPERTY()
	TSet<ESettingsCategory> PendingCategories;
	
	UFUNCTION(BlueprintCallable, Category="Settings")
	bool HasPendingChanges() const
	{
		return PendingCategories.Num() > 0;
	}
	void MarkCategoryPending(ESettingsCategory Category)
	{
		if (Category == ESettingsCategory::None) return;
		if (!PendingCategories.Contains(Category))
		{
			PendingCategories.Add(Category);
			OnSettingsChanged.Broadcast(Category);
		}
	}

	void ClearCategoryPending(ESettingsCategory Category)
	{
		if (PendingCategories.Remove(Category) > 0)
		{
			OnSettingsChanged.Broadcast(Category);
		}
	}

};

