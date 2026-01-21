#pragma once

#include "UI/Base/UIBaseWidget.h"
#include "MenuSliderWidget.generated.h"

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

	UFUNCTION(BlueprintCallable)
	float GetValue() const;

	UPROPERTY(BlueprintAssignable)
	FOnMenuSliderValueChanged OnValueChanged;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnPreConstruct();

	UFUNCTION()
	void HandleSliderChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	USlider* Slider;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Label;

	//Designer-editabledefault label
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MenuSlider")
	FText DesignerLabel = FText::FromString("Slider");
};

