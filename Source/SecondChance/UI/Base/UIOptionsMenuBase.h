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

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* GraphicsTab;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* ControlsTab;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* GameplayTab;

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

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* CancelButton;

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

