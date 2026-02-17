#include "UI/Base/UBaseLabeledWidget.h"

void UBaseLabeledWidget::SetLabel(FText InText)
{
	if (LabelText)
	{
		LabelText->SetText(InText);
	}
}

void UBaseLabeledWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
    
	if (LabelText) // Vienmēr pārbaudi pointeri!
	{
		if (!LabelDefaultValue.IsEmpty())
		{
			LabelText->SetText(LabelDefaultValue);
		}
	}
}