#pragma once

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
	void HandleAudioTab();
	void HandleGraphicsTab();
	void HandleControlsTab();
	void HandleGameplayTab();

	void HandleApply();
	void HandleCancel();

	void SetActiveCategory(ESettingsCategory Category);
};

