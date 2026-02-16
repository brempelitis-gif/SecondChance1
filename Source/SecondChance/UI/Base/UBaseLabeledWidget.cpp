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
    
	// Ja editorā ierakstīsi tekstu "LabelDefaultValue", tas automātiski parādīsies
	if (LabelText && !LabelDefaultValue.IsEmpty())
	{
		LabelText->SetText(LabelDefaultValue);
	}
}