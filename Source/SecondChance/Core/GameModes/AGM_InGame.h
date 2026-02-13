#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGM_InGame.generated.h"

UCLASS()
class SECONDCHANCE_API AGM_InGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_InGame();
	virtual void BeginPlay() override;
	void TakeStartScreenshot();
};
