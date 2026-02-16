#pragma once

#include "CoreMinimal.h"
#include "UI/Base/UBaseLabeledWidget.h" // Bāzes klase
#include "Components/ComboBoxString.h"
#include "MenuDropdownWidget.generated.h"

class UComboBoxString;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnComboBoxSelectionEvent,
	FString, SelectedItem,
	ESelectInfo::Type, SelectType
);

UCLASS()
class SECONDCHANCE_API UMenuDropdownWidget : public UBaseLabeledWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="MenuDropdown")
	void SetOptions(const TArray<FString>& InOptions);

	UFUNCTION(BlueprintCallable, Category="MenuDropdown")
	void SetSelectedIndex(int32 Index);

	UFUNCTION(BlueprintPure, Category="MenuDropdown")
	int32 GetSelectedIndex() const;

	void ClearOptions();
	void AddOption(const FString& Option) const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnComboBoxSelectionEvent OnSelectionChanged;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectType);

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox;

	/* Šeit mēs izmantojam tikai tos variantus, ko gribam redzēt dropdown sarakstā */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MenuDropdown")
	TArray<FString> DesignerOptions;

private:
	TArray<FString> CachedOptions;
};