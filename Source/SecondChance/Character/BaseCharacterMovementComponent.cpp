#include "BaseCharacterMovementComponent.h"

void UBaseCharacterMovementComponent::SetDesiredGait(EGait Gait)
{
	ApplyGait(CurrentGait);
	CurrentGait = DesiredGait;
	DesiredGait = Gait;
}

bool UBaseCharacterMovementComponent::ApplyGait(EGait Gait)
{
	if (!GaitSettings.Contains(Gait))
	{
		return false;
	}
	const FGaitParams& Params = GaitSettings[Gait];
	
	MaxWalkSpeed = Params.MaxWalkSpeed;
	MaxAcceleration = Params.MaxAcceleration;
	BrakingDecelerationWalking = Params.BrakingDecelerationWalking;
	BrakingFrictionFactor = Params.BrakingFrictionFactor;
	bUseSeparateBrakingFriction = Params.bUseSeperateBrakingFriction;

	return true;
}
