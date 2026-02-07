#include "MenuSliderWidget.h"

#include <string>

#include "Components/Slider.h"
#include "Components/TextBlock.h"

void UMenuSliderWidget::SetLabel(const FText& InText)
{
	if (LabelText)
	{
		LabelText->SetText(InText);
	}
}

void UMenuSliderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Slider)
	{
		Slider->OnValueChanged.AddDynamic(
			this, &UMenuSliderWidget::HandleSliderChanged
		);
	}
	Value->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMenuSliderWidget::HandleSliderChanged(float InValue)
{
	OnValueChanged.Broadcast(InValue);
	SetValueUI(InValue);
}
void UMenuSliderWidget::SetValue(float InValue)
{
	// Nodrošinām, ka vērtība ir diapazonā 0-1
	float ClampedValue = FMath::Clamp(InValue, 0.f, 1.f);

	if (Slider)
	{
		Slider->SetValue(ClampedValue);
	}
    
	// Obligāti atjaunojam teksta procentu (piem. 50%)
	SetValueUI(ClampedValue);
}
void UMenuSliderWidget::SetValueUI(float InValue)
{
	if (Value)
	{
		int i = static_cast<int>(InValue * 100);
		FString myString = FString::FromInt(i);// build your string however
		FText t = FText::FromString(myString);
		Value->SetText(t);
	}
}

float UMenuSliderWidget::GetValue() const
{
	return Slider ? Slider->GetValue() : 0.f;
}
