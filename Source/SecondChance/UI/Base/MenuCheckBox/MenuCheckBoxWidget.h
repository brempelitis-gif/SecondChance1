#pragma once

#include "CoreMinimal.h"
#include "UI/Base/UBaseLabeledWidget.h"
#include "Components/CheckBox.h"
#include "MenuCheckBoxWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckBoxValueChanged, bool, bIsChecked);

UCLASS()
class SECONDCHANCE_API UMenuCheckBoxWidget : public UBaseLabeledWidget
{
	GENERATED_BODY()

public:

	// Uzstāda un iegūst stāvokli
	void SetIsChecked(bool bInIsChecked);
	bool IsChecked() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCheckBoxValueChanged OnCheckStateChanged;

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* InternalCheckBox;

	UPROPERTY(EditAnywhere, Category = "Appearance")
	FText Label = FText::FromString("Option");

private:
	UFUNCTION()
	void HandleInternalCheckStateChanged(bool bIsChecked);
};
