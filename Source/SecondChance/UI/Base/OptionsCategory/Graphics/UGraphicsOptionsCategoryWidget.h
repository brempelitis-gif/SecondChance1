#pragma once

#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "Components/ComboBoxString.h"
#include "UGraphicsOptionsCategoryWidget.generated.h"

class UMenuDropdownWidget;
class UUIManagerSubsystem;

/**
 * Graphics options category widget
 */
UCLASS()
class SECONDCHANCE_API UGraphicsOptionsCategoryWidget : public UOptionsCategoryBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	/* Bound dropdowns from the UMG widget */
	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* ResolutionCombo;

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* WindowModeCombo;

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* QualityCombo;

	UFUNCTION()
	void BindDropdowns();
	/* Handlers (must be UFUNCTION with same signature as UMenuDropdownWidget::OnSelectionChanged) */
	UFUNCTION()
	void HandleResolutionChanged(int32 SelectedIndex);

	UFUNCTION()
	void HandleWindowModeChanged(int32 SelectedIndex);

	UFUNCTION()
	void HandleQualityChanged(int32 SelectedIndex);
};