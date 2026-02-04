#pragma once

#include "CoreMinimal.h"
#include "Core/Enums/ESettingsCategory.h"
#include "UI/Base/UIBaseWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "UIOptionsMenuBase.generated.h"

class UMenuButtonWidget;
class UWidgetSwitcher;

UCLASS()
class SECONDCHANCE_API UUIOptionsMenuBase : public UUIBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	
	/* =======================
	 * TOP BUTTONS
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

	// --- Jauns: Popup loģika ---
	UPROPERTY(EditAnywhere, Category = "UI|Settings")
	TSubclassOf<class UUIConfirmationPopup> PopupClass;

	UPROPERTY()
	UUIConfirmationPopup* CachedPopup;

	// --- Iekšējais stāvoklis ---
	ESettingsCategory CurrentCategory = ESettingsCategory::Audio;

	/* =======================
	 * CONTENT SWITCHER
	 * ======================= */
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* CategorySwitcher;

	/* =======================
	 * BOTTOM ACTIONS
	 * ======================= */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* ApplyButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText ApplyButtonLabel = FText::FromString("Apply");

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* CancelButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText CancelButtonLabel = FText::FromString("Cancel");

private:
	UPROPERTY()
	UUIManagerSubsystem* UIManager;

	void BindButtons();

	/* === Handlers === */
	UFUNCTION(BlueprintCallable)
	void HandleAudioTab();
	UFUNCTION(BlueprintCallable)
	void HandleGraphicsTab();
	UFUNCTION(BlueprintCallable)
	void HandleControlsTab();
	UFUNCTION(BlueprintCallable)
	void HandleGameplayTab();

	UFUNCTION(BlueprintCallable)
	void HandleApply();
	UFUNCTION(BlueprintCallable)
	void HandleCancel();
	
	// Callback for when settings change in the manager
	UFUNCTION()
	void HandleSettingsChanged(ESettingsCategory ChangedCategory);
	
	// Set active category in the switcher
	void SetActiveCategory(ESettingsCategory Category);

protected:
	// Update visibility of Apply/Cancel buttons based on pending changes
	UFUNCTION(BlueprintCallable, Category="Options")
	virtual void UpdateActionButtonsVisibility();
};

