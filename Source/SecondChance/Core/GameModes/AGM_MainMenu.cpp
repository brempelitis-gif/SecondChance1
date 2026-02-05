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

	UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	if (UIMan && UIMan->UIConfig && UIMan->UIConfig->MainMenuClass)
	{
		// Izveidojam Main Menu caur mūsu jauno sistēmu
		UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(), UIMan->UIConfig->MainMenuClass);
		if (MainMenu)
		{
			UIMan->PushWidget(MainMenu);
			UE_LOG(LogTemp, Log, TEXT("AGM_Menu: Main Menu uzstumts uz ekrāna."));
		}
	}
}