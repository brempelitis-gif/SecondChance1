#include "PawnGameplayComponent.h"
#include "Input/BaseInputComponent.h"
#include "BaseCharacterMovementComponent.h"
#include "BaseGameCharacter.h"
#include "AbilitySystem/BaseGameplayTags.h"
#include "Character/PawnExtensionComponent.h"
#include "Character/BasePawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include  "Character/BasePlayerState.h"
#include  "AbilitySystem/BaseAbilitySystemComponent.h"

const FName UPawnGameplayComponent::Name_ActorFeatureName = TEXT("CharacterGameplay");

void UPawnGameplayComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;

	if (UPawnExtensionComponent* PawnExt = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UBasePawnData* PawnData = PawnExt->GetPawnData<UBasePawnData>())
		{
			if (const UBaseInputConfig* InputConfig = PawnData->InputConfig)
			{
				if (UBaseInputComponent* IC = Cast<UBaseInputComponent>(PlayerInputComponent))
				{
					IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased);
					
					IC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
					IC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Run, ETriggerEvent::Started, this, &ThisClass::Input_Run);
					IC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_Crouch, ETriggerEvent::Started, this, &ThisClass::Input_Crouch);
					IC->BindNativeAction(InputConfig, BaseGameplayTags::InputTag_LookMouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse);
				}
			}
		}
	}
}


void UPawnGameplayComponent::OnRegister()
{
	Super::OnRegister();
	if (GetPawn<APawn>())
	{
		RegisterInitStateFeature();
	}
}

void UPawnGameplayComponent::BeginPlay()
{
	Super::BeginPlay();
	BindOnActorInitStateChanged(UPawnExtensionComponent::Name_ActorFeatureName, FGameplayTag(), false);
	TryToChangeInitState(BaseGameplayTags::InitState_Spawned);
	CheckDefaultInitialization();
}

void UPawnGameplayComponent::Input_AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (UPawnExtensionComponent* PawnExt = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UBaseAbilitySystemComponent* ASC = PawnExt->GetBaseAbilitySystemComponent())
			{
				ASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void UPawnGameplayComponent::Input_AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (UPawnExtensionComponent* PawnExt = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UBaseAbilitySystemComponent* ASC = PawnExt->GetBaseAbilitySystemComponent())
			{
				ASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}

bool UPawnGameplayComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return false;
	//-----------------------Spawned-----------------------
	if (!CurrentState.IsValid() && DesiredState == BaseGameplayTags::InitState_Spawned)
	{
		if (Pawn->IsLocallyControlled() && !Pawn->IsBotControlled())
		{
			APlayerController* PC = GetController<APlayerController>();
			if (!Pawn->InputComponent || !PC || !PC->GetLocalPlayer()) 
			{
				return false;
			}
		}
		return true;
	}
	//-----------------------DataAvailable-----------------------
	if (CurrentState == BaseGameplayTags::InitState_Spawned && DesiredState == BaseGameplayTags::InitState_DataAvailable)
	{
		return true;
	}
	//-----------------------DataInitialized-----------------------
	if (CurrentState == BaseGameplayTags::InitState_DataAvailable && DesiredState == BaseGameplayTags::InitState_DataInitialized)
	{
		return  Manager->HasFeatureReachedInitState(Pawn, UPawnExtensionComponent::Name_ActorFeatureName, BaseGameplayTags::InitState_DataAvailable);
	}
	//-----------------------GameplayReady-----------------------
	if (CurrentState == BaseGameplayTags::InitState_DataInitialized && DesiredState == BaseGameplayTags::InitState_GameplayReady)
	{
		return true;
	}
	return false;
}

void UPawnGameplayComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if (CurrentState == BaseGameplayTags::InitState_DataAvailable && DesiredState == BaseGameplayTags::InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();

		if (!(Pawn && PS)){return;}
		
		if (UPawnExtensionComponent* PawnExt = UPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnExt->InitializeAbilitySystem(PS->GetBaseAbilitySystemComponent(), PS);
		}
		if (APlayerController* PC = GetController<APlayerController>())
		{
			if (Pawn->InputComponent)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

void UPawnGameplayComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if (Params.FeatureName == UPawnExtensionComponent::Name_ActorFeatureName)
	{
		if (Params.FeatureState == BaseGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

void UPawnGameplayComponent::CheckDefaultInitialization()
{
	const TArray<FGameplayTag> StateChain = {
		BaseGameplayTags::InitState_Spawned,
		BaseGameplayTags::InitState_DataAvailable,
		BaseGameplayTags::InitState_DataInitialized,
		BaseGameplayTags::InitState_GameplayReady
	};
	ContinueInitStateChain(StateChain);
}

void UPawnGameplayComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn) return;
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}
	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.Y);
	}
}

void UPawnGameplayComponent::Input_Move(const FInputActionValue& Value)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;
	if (!Controller)  return;

	const FVector2D InputActionValue =  Value.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
	if (InputActionValue.X !=0)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		Pawn->AddMovementInput(MovementDirection, InputActionValue.X);
	}
	if (InputActionValue.Y !=0)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		Pawn->AddMovementInput(MovementDirection, InputActionValue.Y);
	}
}

void UPawnGameplayComponent::Input_Crouch()
{
	if (ABaseGameCharacter* Character = GetPawn<ABaseGameCharacter>())
	{
		Character->ToggleCrouch();
		DesiredGait = EGait::Walk;
		UpdateGait();
	}
}

void UPawnGameplayComponent::Input_Run()
{
	if (ABaseGameCharacter* Character = GetPawn<ABaseGameCharacter>())
	{
		UBaseCharacterMovementComponent* MovementComp = Cast<UBaseCharacterMovementComponent>(Character->GetCharacterMovement());
		if (!MovementComp) return;

		DesiredGait = MovementComp->GetCurrentGait() == EGait::Run ? EGait::Walk : EGait::Run;

		UpdateGait();
	}
}

void UPawnGameplayComponent::UpdateGait()
{
	ABaseGameCharacter* Character = GetPawn<ABaseGameCharacter>();
	if (DesiredGait == EGait::Run && Character->bIsCrouched)
	{
		Character->ToggleCrouch();
	}
	if ((uint8)DesiredGait > (uint8)MaxAllowedGait)
	{
		DesiredGait = MaxAllowedGait;
	}
	UBaseCharacterMovementComponent* MovementComp = Cast<UBaseCharacterMovementComponent>(Character->GetCharacterMovement());
	MovementComp->SetDesiredGait(DesiredGait);
}

