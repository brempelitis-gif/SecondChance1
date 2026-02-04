#include "UIManagerSubsystem.h"

#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Automātiska UIConfig piesaiste no GameInstance
	if (!UIConfig)
	{
		if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
		{
			UIConfig = GI->UIConfig;
		}
	}
}
void UUIManagerSubsystem::PushWidget(UUserWidget* NewWidget, bool bShowCursor, bool bPauseGame)
{
	if (!NewWidget) return;

	// 1. Pievienojam ekrānam
	NewWidget->AddToViewport();
	WidgetStack.Push(NewWidget);

	// 2. Ja nepieciešams, nopauzējam spēli
	if (bPauseGame)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	// 3. Automātiski sakārtojam peli un fokusu
	UpdateInputMode();
}

void UUIManagerSubsystem::PopWidget()
{
	if (WidgetStack.Num() == 0) return;

	UUserWidget* TopWidget = WidgetStack.Pop();
	if (TopWidget)
	{
		TopWidget->RemoveFromParent();
	}

	// Ja pēc loga aizvēršanas steks ir tukšs, varam atpauzēt spēli
	if (WidgetStack.Num() == 0)
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

	UpdateInputMode();
}

void UUIManagerSubsystem::UpdateInputMode()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (WidgetStack.Num() > 0)
	{
		// Ja ir atvērts kaut viens logrīks
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetStack.Last()->TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}
	else
	{
		// Ja neviens logrīks nav atvērts - atgriežamies spēlē
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
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