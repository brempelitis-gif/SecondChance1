#include "BaseGameplayAbility_Jump.h"
#include "Character/BaseGameCharacter.h"

void UBaseGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseGameplayAbility_Jump::CharacterJumpStart()
{
	if (ABaseGameCharacter* BaseGameCharacter = GetBaseCharacterFromActorInfo())
	{
		if (BaseGameCharacter->IsLocallyControlled() && !BaseGameCharacter->bPressedJump)
		{
			BaseGameCharacter->UnCrouch();
			BaseGameCharacter->Jump();
		}
	}
}

void UBaseGameplayAbility_Jump::CharacterJumpStop()
{
	if (ABaseGameCharacter* BaseGameCharacter = GetBaseCharacterFromActorInfo())
	{
		if (BaseGameCharacter->IsLocallyControlled() && !BaseGameCharacter->bPressedJump)
		{
			BaseGameCharacter->StopJumping();
		}
	}
}
