#pragma once


#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Core/Enums/MovementStateTypes.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "PawnGameplayComponent.generated.h"

class UBaseInputConfig;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SECONDCHANCE_API UPawnGameplayComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
protected:
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	virtual void OnRegister() override;
	virtual void BeginPlay() override;

	void Input_AbilityInputTagPressed(const FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(const FGameplayTag InputTag);
public:
	static const FName Name_ActorFeatureName;

	virtual FName GetFeatureName() const override { return Name_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
private:
	EGait CurrentGait = EGait::Walk;
	EGait DesiredGait = EGait::Walk;
	EGait MaxAllowedGait = EGait::Run;

public:
	
	UFUNCTION(BlueprintCallable, Category="Pawn Gameplay")
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintCallable, Category="Pawn Gameplay")
	void Input_Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category="Pawn Gameplay")
	void Input_Crouch();

	UFUNCTION(BlueprintCallable, Category="Pawn Gameplay")
	void Input_Run();

	void UpdateGait();
};

