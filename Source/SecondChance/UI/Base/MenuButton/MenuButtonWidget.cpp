#include "MenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


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


void UMenuButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (HoverSound)
	{
		UGameplayStatics::PlaySound2D(this, HoverSound);
	}
}

FReply UMenuButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// Atskaņojam skaņu
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}

	// Izsaucam bāzes klases loģiku un atgriežam tās rezultātu
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
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

