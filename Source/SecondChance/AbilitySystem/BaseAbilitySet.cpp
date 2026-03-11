#include "BaseAbilitySet.h"
#include "Abilities/BaseGameplayAbility.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

void UBaseAbilitySet::GiveToAbilitySystem(UBaseAbilitySystemComponent* ASC, UObject* SourceObject) const
{
	check(ASC);
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		FBaseAbilitySet_GameplayAbility AbilityToGrant(GrantedGameplayAbilities[AbilityIndex]);
		if (!IsValid(AbilityToGrant.Ability)) continue;

		UBaseGameplayAbility* CDO = AbilityToGrant.Ability->GetDefaultObject<UBaseGameplayAbility>();
		FGameplayAbilitySpec AbilitySpec(CDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.inputTag);

		ASC->GiveAbility(AbilitySpec);
	}
}
