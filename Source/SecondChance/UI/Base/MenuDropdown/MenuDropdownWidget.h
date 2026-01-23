#pragma once

#include "UI/Base/UIBaseWidget.h"
#include "MenuDropdownWidget.generated.h"

class UComboBoxString;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnMenuDropdownChanged,
	int32,
	SelectedIndex
);

/**
 * Dropdown wrapper used across options panels.
 * - DesignerOptions and DesignerLabel are editable in the Widget Blueprint defaults.
 * - NativePreConstruct applies Designer values so they appear in the UMG Designer preview.
 */
UCLASS()
class SECONDCHANCE_API UMenuDropdownWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	/* Set options programmatically (or in NativePreConstruct from DesignerOptions) */
	UFUNCTION(BlueprintCallable, Category="MenuDropdown")
	void SetOptions(const TArray<FString>& InOptions);

	/* Set active index */
	UFUNCTION(BlueprintCallable, Category="MenuDropdown")
	void SetSelectedIndex(int32 Index);

	/* Get active index */
	UFUNCTION(BlueprintCallable, Category="MenuDropdown")
	int32 GetSelectedIndex() const;

	/* Set label text (applies to bound Label text block if present) */
	UFUNCTION(BlueprintCallable, Category="MenuDropdown")
	void SetLabel(const FText& InText);

	UPROPERTY(BlueprintAssignable, Category="MenuDropdown")
	FOnMenuDropdownChanged OnSelectionChanged;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectType);

	/* Bound widgets from UMG */
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Label;

	/* Designer-exposed defaults so the UMG Blueprint class defaults show options & label */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MenuDropdown")
	TArray<FString> DesignerOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MenuDropdown")
	FText DesignerLabel;

private:
	/* Local cache of options to map selected string -> index */
	TArray<FString> CachedOptions;
};