#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"

void UMenuCheckBoxWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (LabelText) LabelText->SetText(Label);
}

void UMenuCheckBoxWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (InternalCheckBox)
	{
		InternalCheckBox->OnCheckStateChanged.AddDynamic(this, &UMenuCheckBoxWidget::HandleInternalCheckStateChanged);
	}
}

void UMenuCheckBoxWidget::SetLabel(FText InText)
{
	Label = InText;
	if (LabelText) LabelText->SetText(Label);
}

void UMenuCheckBoxWidget::SetIsChecked(bool bInIsChecked)
{
	if (InternalCheckBox) InternalCheckBox->SetCheckedState(bInIsChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

bool UMenuCheckBoxWidget::IsChecked() const
{
	return InternalCheckBox ? InternalCheckBox->IsChecked() : false;
}

void UMenuCheckBoxWidget::HandleInternalCheckStateChanged(bool bIsChecked)
{
	OnCheckStateChanged.Broadcast(bIsChecked);
}
