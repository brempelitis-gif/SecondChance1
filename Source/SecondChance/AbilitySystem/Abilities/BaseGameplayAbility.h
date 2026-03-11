#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

class ABaseGameCharacter;

UENUM(BlueprintType)
enum class EBaseAbilityActivationPolicy : uint8
{
	OnInputTriggered,
	WhileInputActive
};

UCLASS()
class SECONDCHANCE_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	EBaseAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	ABaseGameCharacter* GetBaseCharacterFromActorInfo() const;
protected:
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Activation")
	EBaseAbilityActivationPolicy ActivationPolicy;
	
};
