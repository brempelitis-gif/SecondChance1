#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "MenuCheckBoxWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckBoxValueChanged, bool, bIsChecked);

UCLASS()
class SECONDCHANCE_API UMenuCheckBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Uzstāda tekstu blakus checkboxam
	void SetLabel(FText InText);

	// Uzstāda un iegūst stāvokli
	void SetIsChecked(bool bInIsChecked);
	bool IsChecked() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCheckBoxValueChanged OnCheckStateChanged;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* InternalCheckBox;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FText Label = FText::FromString("Option");

private:
	UFUNCTION()
	void HandleInternalCheckStateChanged(bool bIsChecked);
};
