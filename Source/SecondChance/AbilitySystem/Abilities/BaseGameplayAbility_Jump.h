#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "BaseGameplayAbility_Jump.generated.h"

UCLASS()
class SECONDCHANCE_API UBaseGameplayAbility_Jump : public UBaseGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable, Category="Ability")
	void CharacterJumpStart();
	
	UFUNCTION(BlueprintCallable, Category="Ability")
	void CharacterJumpStop();
	
};

