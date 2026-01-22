#include "MenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UMenuButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button)
	{
		Button->OnClicked.AddDynamic(
			this,
			&UMenuButtonWidget::HandleButtonClicked
		);
	}
}

void UMenuButtonWidget::HandleButtonClicked()
{
	OnClicked.Broadcast();
}

void UMenuButtonWidget::SetEnabled(bool bEnabled)
{
	if (Button)
	{
		Button->SetIsEnabled(bEnabled);
	}
}

void UMenuButtonWidget::SetLabel(const FText& InText)
{
	if (LabelText)
	{
		LabelText->SetText(InText);
	}
}

