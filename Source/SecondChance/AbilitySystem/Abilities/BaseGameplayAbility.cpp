#include "BaseGameplayAbility.h"
#include "Character/BaseGameCharacter.h"

ABaseGameCharacter* UBaseGameplayAbility::GetBaseCharacterFromActorInfo() const
{
	return CurrentActorInfo ? Cast<ABaseGameCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr;
}

void UBaseGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
