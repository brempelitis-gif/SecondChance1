#pragma once
#include "CoreMinimal.h"
#include "Core/Enums/MovementStateTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

UCLASS()
class SECONDCHANCE_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
protected:
	
	EGait CurrentGait = EGait::Walk;
	EGait DesiredGait = EGait::Walk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	TMap<EGait, FGaitParams> GaitSettings;
	
public:
	
	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
	EGait GetCurrentGait() const { return CurrentGait; }

	UFUNCTION(BlueprintCallable, Category = "CharacterMovement")
	void SetDesiredGait(EGait Gait);
	
protected:
	
	bool ApplyGait(EGait Gait);
};