#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGM_Game.generated.h"

class APlayerController;
class UBasePawnData;

UCLASS()
class SECONDCHANCE_API AGM_Game : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	const UBasePawnData* GetPawnDataForController(const AController* InController) const;



	virtual void BeginPlay() override;
};
