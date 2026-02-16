#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"

void UMenuCheckBoxWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (InternalCheckBox)
	{
		InternalCheckBox->OnCheckStateChanged.AddDynamic(this, &UMenuCheckBoxWidget::HandleInternalCheckStateChanged);
	}
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
