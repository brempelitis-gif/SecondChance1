#pragma once

#include "UI/Base/UIBaseWidget.h"
#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "MenuDropdownWidget.generated.h"

class UComboBoxString;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnComboBoxSelectionEvent,
	FString,
	SelectedIItem,
	ESelectInfo::Type, SelectType
);
UCLASS()
class SECONDCHANCE_API UMenuDropdownWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	void ClearOptions();
	void AddOption(const FString& Option) const;
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

	// Delegāts, kuram pieslēgsies GraphicsWidget
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnComboBoxSelectionEvent OnSelectionChanged;

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


