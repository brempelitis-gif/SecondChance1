#include "Core/Subsystems/UIManagerSubsystem.h"
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (!UIConfig)
	{
		if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
		{
			UIConfig = GI->UIConfig;
		}
	}
	if (UIConfig)
	{
		UE_LOG(LogTemp, Log, TEXT("UIManager: UIConfig ielādēts veiksmīgi!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UIManager: UIConfig NAV ATRASTS! UI nevarēs darboties."));
	}
}
UUIConfirmationPopup* UUIManagerSubsystem::PushConfirmationPopup(FText CategoryName, float Timeout)
{
	if (!UIConfig || !UIConfig->ConfirmationPopupClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UIManager: ConfirmationPopupClass nav iestatīta UIConfigā!"));
		return nullptr;
	}

	// 1. Izveidojam logrīku
	UUIConfirmationPopup* Popup = CreateWidget<UUIConfirmationPopup>(GetWorld(), UIConfig->ConfirmationPopupClass);
    
	if (Popup)
	{
		// 2. Izmantojam tavu esošo Push loģiku!
		// bShowCursor = true, bPauseGame = false (jo opciju menu jau parasti ir nopauzēts)
		PushWidget(Popup, true, false);
        
		// 3. Inicializējam Pop-up datus
		Popup->ShowPopup(CategoryName, Timeout);
	}

	return Popup;
}

void UUIManagerSubsystem::PushWidget(UUserWidget* NewWidget, bool bShowCursor, bool bPauseGame)
{
	if (!NewWidget) return;
	NewWidget->AddToViewport();
	WidgetStack.Push(NewWidget);

	if (bPauseGame) UGameplayStatics::SetGamePaused(GetWorld(), true);
	UpdateInputMode();
}

void UUIManagerSubsystem::PopWidget()
{
	if (WidgetStack.Num() == 0) return;
	UUserWidget* TopWidget = WidgetStack.Pop();
	if (TopWidget) TopWidget->RemoveFromParent();

	if (WidgetStack.Num() == 0) UGameplayStatics::SetGamePaused(GetWorld(), false);
	UpdateInputMode();
}

void UUIManagerSubsystem::UpdateInputMode()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (WidgetStack.Num() > 0)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetStack.Last()->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

void UUIManagerSubsystem::ClearAllWidgets()
{
	for (UUserWidget* Widget : WidgetStack)
	{
		if (Widget) Widget->RemoveFromParent();
	}
	WidgetStack.Empty();
	UpdateInputMode();
}