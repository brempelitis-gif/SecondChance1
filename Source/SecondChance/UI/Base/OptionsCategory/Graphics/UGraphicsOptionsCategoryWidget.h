#pragma once

#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "Components/ComboBoxString.h"
#include "UGraphicsOptionsCategoryWidget.generated.h"

class UComboBoxString;
class UMenuDropdownWidget;

/**
 * Graphics options category widget.
 * - Bind three ComboBoxString widgets (Resolution / Window Mode / Quality) in the WBP.
 * - Uses UIManagerSubsystem to preview and mark pending changes.
 */
UCLASS()
class SECONDCHANCE_API UGraphicsOptionsCategoryWidget : public UOptionsCategoryBaseWidget
{
	GENERATED_BODY()

protected:
	/* Bind these in the UMG Blueprint (names must match unless you change them) */
	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* ResolutionCombo;

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* WindowModeCombo;

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* QualityCombo;

	/* Cached strings for resolution entries (same order as UIManager->SupportedResolutions) */
	UPROPERTY()
	TArray<FString> CachedResolutionStrings;

	/* Suppress callbacks while programmatically changing selections */
	bool bSuppressEvents = false;

	/* UWidget lifecycle */
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	/* Populate comboboxes from UIManager (supported resolutions etc.) */
	void PopulateCombos();

	/* Refresh UI from UIManager pending values */
	void RefreshFromManager();

	/* Combo handlers */
	UFUNCTION()
	void OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnWindowModeSelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnQualitySelected(FString SelectedItem, ESelectInfo::Type SelectionType);

	/* React when UIManager broadcasts settings changes (apply/cancel or other updates) */
	UFUNCTION()
	void HandleManagerSettingsChanged(ESettingsCategory ChangedCategory);
};