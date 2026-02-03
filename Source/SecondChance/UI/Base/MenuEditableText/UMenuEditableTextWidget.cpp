#include "UI/Base/MenuEditableText/UMenuEditableTextWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"

void UMenuEditableTextWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (LabelText) LabelText->SetText(LabelValue);
}

void UMenuEditableTextWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (EditableInput)
	{
		EditableInput->OnTextChanged.AddDynamic(this, &UMenuEditableTextWidget::HandleOnTextChanged);
	}
}

void UMenuEditableTextWidget::SetLabel(FText InText) { if (LabelText) LabelText->SetText(InText); }

void UMenuEditableTextWidget::SetText(FText InText) { if (EditableInput) EditableInput->SetText(InText); }

FText UMenuEditableTextWidget::GetText() const { return EditableInput ? EditableInput->GetText() : FText::GetEmpty(); }

void UMenuEditableTextWidget::HandleOnTextChanged(const FText& Text)
{
	OnTextChanged.Broadcast(Text);
}