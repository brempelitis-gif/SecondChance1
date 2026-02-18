#include "UI/HUD/SecondChanceHUD.h"
#include "UI/HUD/MainHUDWidget.h"
#include "Blueprint/UserWidget.h"

void ASecondChanceHUD::BeginPlay()
{
	Super::BeginPlay();

	// 1. Pārbaudām, vai klase ir iestatīta (WBP_MainHUD)
	if (MainHUDClass)
	{
		// 2. Izveidojam Widget instanci
		MainHUD = CreateWidget<UMainHUDWidget>(GetOwningPlayerController(), MainHUDClass);

		if (MainHUD)
		{
			// 3. Pievienojam ekrānam
			MainHUD->AddToViewport();
            
			UE_LOG(LogTemp, Log, TEXT("HUD: Galvenais panelis veiksmīgi izveidots."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HUD: MainHUDClass nav norādīta! Pārbaudi savu HUD Blueprint."));
	}
}