#include "UIConfirmationPopup.h"
#include "Components/TextBlock.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"

void UUIConfirmationPopup::ShowPopup(FText categoryName, float timeout)
{
	BaseDescriptionText = FText::Format(NSLOCTEXT("UI", "PopupDesc", "Do you want to keep changes in {0}?"), categoryName);
	if (DescriptionLabel) DescriptionLabel->SetText(BaseDescriptionText);

	CurrentTimeRemaining = timeout;
	bIsTimerActive = true;
	
	SetVisibility(ESlateVisibility::Visible);

	if (AcceptButton) AcceptButton->OnClicked.AddUniqueDynamic(this, &UUIConfirmationPopup::HandleAccept);
	if (RevertButton) RevertButton->OnClicked.AddUniqueDynamic(this, &UUIConfirmationPopup::HandleRevert);
}

void UUIConfirmationPopup::HidePopup()
{
	bIsTimerActive = false;
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUIConfirmationPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsTimerActive)
	{
		CurrentTimeRemaining -= InDeltaTime;
		if (TimerLabel)
		{
			TimerLabel->SetText(FText::AsNumber(FMath::Max(0, FMath::FloorToInt(CurrentTimeRemaining))));
		}

		if (CurrentTimeRemaining <= 0.0f)
		{
			HandleRevert();
		}
	}
}

void UUIConfirmationPopup::HandleAccept()
{
	bIsTimerActive = false;
	OnConfirmed.Broadcast();
	HidePopup();
}

void UUIConfirmationPopup::HandleRevert()
{
	bIsTimerActive = false;
	OnTimedOutOrCancelled.Broadcast();
	HidePopup();
}