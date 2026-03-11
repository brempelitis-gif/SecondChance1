#include "BasePlayerState.h"
#include "AbilitySystem/BaseAbilitySet.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Character/BasePawnData.h"
#include "AbilitySystem/BaseAbilitySet.h"

ABasePlayerState::ABasePlayerState(const class FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UBaseAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const
{
	return GetBaseAbilitySystemComponent();
}
void ABasePlayerState::SetPawnData(const UBasePawnData* InPawnData)
{
	check(InPawnData);
	if (PawnData)
	{
		return;
	}
	PawnData = InPawnData;
	for (const UBaseAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

void ABasePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	check(AbilitySystemComponent);

	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}
