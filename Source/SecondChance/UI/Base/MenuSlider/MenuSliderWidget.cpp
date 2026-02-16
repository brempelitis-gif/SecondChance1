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
	
	if (Slider) Slider->OnValueChanged.AddDynamic(this, &UMenuSliderWidget::HandleSliderChanged);
    
	if (Value) Value->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMenuSliderWidget::HandleSliderChanged(float InValue)
{
	// 1. Paziņojam citiem (CharacterAppearanceWidget), ka vērtība mainījās
	OnValueChanged.Broadcast(InValue);

	// 2. Mainām tekstu TIKAI tad, ja ir ieslēgts "Auto Update"
	// Audio settingiem šis būs TRUE (rādīs 0-100%)
	// Character Creatoram šis būs FALSE (jo tur mēs paši rēķinām cm un kg)
	if (bAutoUpdateText)
	{
		SetValueUI(InValue);
	}
}
void UMenuSliderWidget::SetValue(float InValue)
{
	float ClampedValue = FMath::Clamp(InValue, 0.f, 1.f);

	if (Slider)
	{
		Slider->SetValue(ClampedValue);
	}
    
	// Ja mēs uzstādām vērtību no koda, mēs parasti gribam atjaunot arī tekstu
	// Bet, ja bAutoUpdateText ir false, mums jāuzmanās, lai nepārrakstītu cm ar %
	if (bAutoUpdateText)
	{
		SetValueUI(ClampedValue);
	}
}
void UMenuSliderWidget::SetValueUI(float InValue)
{
	if (!Value) return;

	float FinalValue = InValue;

	// Ja šis ir procentu slaideris (Audio), reizinām ar 100
	// Ja šis ir augums (Character), mēs jau padodam 180, tāpēc nevajag reizināt
	if (bIsPercentage)
	{
		FinalValue = InValue * 100.0f;
	}

	// Noapaļojam uz veselu skaitli
	int32 IntValue = FMath::RoundToInt(FinalValue);
    
	FString MyString = FString::FromInt(IntValue);
	Value->SetText(FText::FromString(MyString));
}

float UMenuSliderWidget::GetValue() const
{
	return Slider ? Slider->GetValue() : 0.f;
}
