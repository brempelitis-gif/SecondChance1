#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "BaseWorldSettings.generated.h"

class UBasePawnData;
UCLASS()
class SECONDCHANCE_API ABaseWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	const UBasePawnData* GetDefaultPawnData() const {return DefaultPawnData;}
protected:
	UPROPERTY(EditDefaultsOnly, Category= "Pawn")
	TObjectPtr<UBasePawnData> DefaultPawnData;
	
};
