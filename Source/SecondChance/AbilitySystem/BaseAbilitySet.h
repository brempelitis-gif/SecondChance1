#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BaseAbilitySet.generated.h"

class UBaseGameplayAbility;
class UBaseAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FBaseAbilitySet_GameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBaseGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag inputTag;
};

UCLASS(BlueprintType)
class SECONDCHANCE_API UBaseAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	void GiveToAbilitySystem(UBaseAbilitySystemComponent* ASC, UObject* SourceObject) const;
protected:
	UPROPERTY(EditDefaultsOnly, category = "Gameplay Abilities")
	TArray<FBaseAbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
};
