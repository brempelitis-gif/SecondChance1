#include "UIConfirmationPopup.h"
#include "Components/TextBlock.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "TimerManager.h"


void UUIConfirmationPopup::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Uzstādām noklusējuma tekstus, lai Editorā viss izskatītos pareizi
	if (DescriptionLabel)
	{
		DescriptionLabel->SetText(NSLOCTEXT("UI", "DefaultDesc", "Do you want to keep changes?"));
	}
	
	if (TimerLabel)
	{
		TimerLabel->SetText(FText::FromString("15"));
	}

	if (AcceptButton)
	{
		AcceptButton->SetLabel(AcceptButtonLabel);
	}

	if (RevertButton)
	{
		RevertButton->SetLabel(RevertButtonLabel);
	}
}
void UUIConfirmationPopup::ShowPopup(FText categoryName, float timeout)
{
	CachedCategoryName = categoryName;
	SecondsRemaining = FMath::CeilToInt(timeout);
	
	RefreshDescriptionText();
	
	if (TimerLabel)
	{
		TimerLabel->SetText(FText::AsNumber(SecondsRemaining));
	}

	SetVisibility(ESlateVisibility::Visible);

	// Piesaistām pogas (AddUniqueDynamic novērš dubultu piesaisti)
	if (AcceptButton) AcceptButton->OnClicked.AddUniqueDynamic(this, &UUIConfirmationPopup::HandleAccept);
	if (RevertButton) RevertButton->OnClicked.AddUniqueDynamic(this, &UUIConfirmationPopup::HandleRevert);

	// Sākam taimeri - sauksim UpdateTimerTick katru 1.0 sekundi
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UUIConfirmationPopup::UpdateTimerTick, 1.0f, true);
}
void UUIConfirmationPopup::UpdateTimerTick()
{
	SecondsRemaining--;

	if (TimerLabel)
	{
		TimerLabel->SetText(FText::AsNumber(FMath::Max(0, SecondsRemaining)));
	}

	if (SecondsRemaining <= 0)
	{
		HandleRevert();
	}
}
void UUIConfirmationPopup::HidePopup()
{
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUIConfirmationPopup::RefreshDescriptionText()
{
	if (DescriptionLabel)
	{
		FText FormattedDesc = FText::Format(NSLOCTEXT("UI", "PopupDesc", "Do you want to keep changes in {0}?"), CachedCategoryName);
		DescriptionLabel->SetText(FormattedDesc);
	}
}

void UUIConfirmationPopup::HandleAccept()
{
	OnConfirmed.Broadcast();
	HidePopup();
}

void UUIConfirmationPopup::HandleRevert()
{
	OnTimedOutOrCancelled.Broadcast();
	HidePopup();
}

void UUIConfirmationPopup::NativeDestruct()
{
	// Drošības pēc notīrām taimeri, ja logrīks tiek iznīcināts
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	Super::NativeDestruct();
}

