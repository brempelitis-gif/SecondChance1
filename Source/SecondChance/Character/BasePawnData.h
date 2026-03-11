#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BasePawnData.generated.h"

class UBaseInputConfig;
class UBaseAbilitySet;

UCLASS()
class SECONDCHANCE_API UBasePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBasePawnData(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UBaseInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<UBaseAbilitySet>> AbilitySets;
};
