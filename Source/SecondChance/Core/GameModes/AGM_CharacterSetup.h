#pragma once
#include "GameFramework/GameModeBase.h"
#include "AGM_CharacterSetup.generated.h"
UCLASS()
class SECONDCHANCE_API AGM_CharacterSetup : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_CharacterSetup();

protected:
	virtual void BeginPlay() override;
};