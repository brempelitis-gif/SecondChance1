#include "PawnExtensionComponent.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"

const FName UPawnExtensionComponent::Name_ActorFeatureName = TEXT("PawnExtension");

bool UPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	APawn* Pawn = GetPawn<APawn>();
	//-----------------------Spawned-----------------------
	if (!CurrentState.IsValid() && DesiredState == BaseGameplayTags::InitState_Spawned)
	{
		if (Pawn) return true;
	}
	//-----------------------DataAvailable-----------------------
	if (CurrentState == BaseGameplayTags::InitState_Spawned && DesiredState == BaseGameplayTags::InitState_DataAvailable)
	{
		if (!PawnData)
		{
			return false;
		}
		if (Pawn->IsLocallyControlled())
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}
		return true;
	}
	//-----------------------DataInitialized-----------------------
	if (CurrentState == BaseGameplayTags::InitState_DataAvailable && DesiredState == BaseGameplayTags::InitState_DataInitialized)
	{
		return Manager->HaveAllFeaturesReachedInitState(Pawn, BaseGameplayTags::InitState_DataAvailable);
	}
	//-----------------------GameplayReady-----------------------
	if (CurrentState == BaseGameplayTags::InitState_DataInitialized && DesiredState == BaseGameplayTags::InitState_GameplayReady)
	{
		return true;
	}
	return false;
}

void UPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
}

void UPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName != Name_ActorFeatureName)
	{
		if (Params.FeatureState == BaseGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UPawnExtensionComponent::CheckDefaultInitialization()
{
	CheckDefaultInitializationForImplementers();
	const TArray<FGameplayTag> StateChain = {
		BaseGameplayTags::InitState_Spawned,
		BaseGameplayTags::InitState_DataAvailable,
		BaseGameplayTags::InitState_DataInitialized,
		BaseGameplayTags::InitState_GameplayReady
	};
	ContinueInitStateChain(StateChain);

}

void UPawnExtensionComponent::InitializeAbilitySystem(UBaseAbilitySystemComponent* InASC, AActor* InOwner)
{
	check(InASC);
	check(InOwner);
	if (InASC == AbilitySystemComponent)
	{
		return;
	}
	if (AbilitySystemComponent)
	{
		return UnInitializeAbilitySystem();
	}
	APawn* Pawn = GetPawnChecked<APawn>();

	AActor* ExistingAvatar = InASC->GetAvatarActor();
	//multiplayer-network lag situation
	if (ExistingAvatar && ExistingAvatar != Pawn)
	{
		if(UPawnExtensionComponent* PawnExt = FindPawnExtensionComponent(ExistingAvatar))
		{
			PawnExt->UnInitializeAbilitySystem();
		}
	}

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwner, Pawn);
}

void UPawnExtensionComponent::UnInitializeAbilitySystem()
{
	if (!AbilitySystemComponent) return;
	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities();
		if (AbilitySystemComponent->GetAvatarActor())
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			AbilitySystemComponent->ClearActorInfo();
		}
	}
	AbilitySystemComponent = nullptr;
}

void UPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		RegisterInitStateFeature();
	}
}

void UPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);
	TryToChangeInitState(BaseGameplayTags::InitState_Spawned);
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UPawnExtensionComponent::SetPawnData(const UBasePawnData* InPawnData)
{
	check(InPawnData);
	if (PawnData) return;

	PawnData = InPawnData;
}
