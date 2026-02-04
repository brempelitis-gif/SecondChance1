#include "AGM_MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

AGM_MainMenu::AGM_MainMenu()
{
	// Main Menu parasti nav vajadzīgs DefaultPawn (spēlētāja tēls)
	DefaultPawnClass = nullptr;
}

void AGM_MainMenu::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC) return;

	// 1. Izveidojam un parādām logrīku
	if (MainMenuWidgetClass)
	{
		UUserWidget* Menu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		GetGameInstance()->GetSubsystem<UUIManagerSubsystem>()->PushWidget(Menu);
	}
/*
	// 2. Iestatām ievades režīmu tikai uz UI
	FInputModeUIOnly InputMode;
	if (CurrentMainMenuWidget)
	{
		InputMode.SetWidgetToFocus(CurrentMainMenuWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	}

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true; // Obligāti parādām peli  */
}