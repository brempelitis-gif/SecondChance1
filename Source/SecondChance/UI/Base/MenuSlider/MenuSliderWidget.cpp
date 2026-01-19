#include "MenuSliderWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UMenuSliderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Slider)
	{
		Slider->OnValueChanged.AddDynamic(
			this, &UMenuSliderWidget::HandleSliderChanged
		);
	}
}

void UMenuSliderWidget::HandleSliderChanged(float Value)
{
	OnValueChanged.Broadcast(Value);
}

void UMenuSliderWidget::SetValue(float InValue)
{
	if (Slider)
	{
		Slider->SetValue(InValue);
	}
}

float UMenuSliderWidget::GetValue() const
{
	return Slider ? Slider->GetValue() : 0.f;
}
