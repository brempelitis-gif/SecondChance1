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

UCLASS()
class SECONDCHANCE_API UMenuDropdownWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	/* Uzstāda visas opcijas */
	UFUNCTION(BlueprintCallable)
	void SetOptions(const TArray<FString>& InOptions);

	/* Uzstāda aktīvo index */
	UFUNCTION(BlueprintCallable)
	void SetSelectedIndex(int32 Index);

	/* Dabū aktīvo index */
	UFUNCTION(BlueprintCallable)
	int32 GetSelectedIndex() const;
	

	UPROPERTY(BlueprintAssignable)
	FOnMenuDropdownChanged OnSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectType);

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectType);

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Label;

	UPROPERTY(meta = (BindWidget))
	UComboBox* Menu;
private:
	TArray<FString> CachedOptions;
};

