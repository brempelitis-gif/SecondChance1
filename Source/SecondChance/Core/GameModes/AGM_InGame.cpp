#include "AGM_InGame.h"
#include "MyGameInstance.h"
#include "Player/SCharacter.h" // Tavs spēlētāja tēls
#include "Kismet/GameplayStatics.h"

AGM_InGame::AGM_InGame()
{
	// Šeit vari iestatīt DefaultPawn, PlayerController, HUD utt., ja nepieciešams
}

void AGM_InGame::BeginPlay()
{
	Super::BeginPlay();

	// 1. Iegūstam datus no GameInstance
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		// 2. Atrodam spēlētāja tēlu pasaulē
		ASCharacter* PlayerChar = Cast<ASCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        
		if (PlayerChar)
		{
			// 3. Uzstādām izskatu, ko spēlētājs tikko izveidoja
			PlayerChar->ApplyCustomizationData(GI->FinalCharacterData);
            
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
				FString::Printf(TEXT("Welcome, %s! Stats applied."), *GI->FinalCharacterData.PlayerName));
		}
	}
}