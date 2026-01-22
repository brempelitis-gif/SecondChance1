#pragma once
#include "Components/EditableTextBox.h"
#include "UI/Base/UIBaseWidget.h"
#include "MenuSliderWidget.generated.h"

class UTextBox;
class USlider;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnMenuSliderValueChanged,
	float,
	Value
);

UCLASS()
class SECONDCHANCE_API UMenuSliderWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetValue(float InValue);
	void SetValueUI(float InValue);

	UFUNCTION(BlueprintCallable)
	float GetValue() const;

	UPROPERTY(BlueprintAssignable)
	FOnMenuSliderValueChanged OnValueChanged;
	
	UFUNCTION(BlueprintCallable, Category="Label")
	void SetLabel(const FText& InText);
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleSliderChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	USlider* Slider;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Value;
};

