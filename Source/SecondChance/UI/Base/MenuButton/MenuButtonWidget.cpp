#include "MenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UMenuButtonWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (Button)
	{
		// Standarta klikšķis
		Button->OnClicked.AddDynamic(this, &UMenuButtonWidget::HandleButtonClicked);
       
		// Jaunie notikumi nepārtrauktai darbībai (piem. rotācijai)
		Button->OnPressed.AddDynamic(this, &UMenuButtonWidget::HandleButtonPressed);
		Button->OnReleased.AddDynamic(this, &UMenuButtonWidget::HandleButtonReleased);
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
// ---  Handleri, kas izsauc Broadcast ---

void UMenuButtonWidget::HandleButtonPressed()
{
	OnPressed.Broadcast();
}

void UMenuButtonWidget::HandleButtonReleased()
{
	OnReleased.Broadcast();
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

