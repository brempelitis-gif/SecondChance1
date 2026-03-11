#include "AGM_Game.h"
#include "BaseWorldSettings.h"
#include "Character/BasePlayerState.h"
#include "Character/PawnExtensionComponent.h"
#include "MyGameInstance.h"
#include "Character/BaseGameCharacter.h" // Tavs spēlētāja tēls
#include "Kismet/GameplayStatics.h"

void AGM_Game::PostLogin(APlayerController* NewPlayer)
{
	if (const ABaseWorldSettings* WS = Cast<ABaseWorldSettings>(GetWorld()->GetWorldSettings()))
	{
		if (const UBasePawnData* PawnData = WS->GetDefaultPawnData())
		{
			if (ABasePlayerState* PS = NewPlayer->GetPlayerState<ABasePlayerState>())
			{
				PS->SetPawnData(PawnData);
			}
		}
	}
	Super::PostLogin(NewPlayer);
}

APawn* AGM_Game::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if(UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			if (UPawnExtensionComponent* PawnExt = UPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				PawnExt->SetPawnData(GetPawnDataForController(NewPlayer));
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}
	return nullptr;
}

const UBasePawnData* AGM_Game::GetPawnDataForController(const AController* InController) const
{
	if (InController)
	{
		if (const ABasePlayerState* PS = InController->GetPlayerState<ABasePlayerState>())
		{
			if (const UBasePawnData* PawnData = PS->GetPawnData<UBasePawnData>())
			{
				return PawnData;
			}
		}
	}
	return nullptr;
}

void AGM_Game::BeginPlay()
{
	Super::BeginPlay();

	// 1. Iegūstam datus no GameInstance
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		// 2. Atrodam spēlētāja tēlu pasaulē
		ABaseGameCharacter* PlayerChar = Cast<ABaseGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        
		if (PlayerChar)
		{
			// 3. Uzstādām izskatu, ko spēlētājs tikko izveidoja
			PlayerChar->ApplyCustomizationData(GI->FinalCharacterData);
            
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, 
				FString::Printf(TEXT("Welcome, %s! Stats applied."), *GI->FinalCharacterData.PlayerName));
		}
	}
}