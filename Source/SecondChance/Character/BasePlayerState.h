#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "BasePlayerState.generated.h"

class UBaseAbilitySystemComponent;
class UBasePawnData;

UCLASS()
class SECONDCHANCE_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	ABasePlayerState(const class FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const {return AbilitySystemComponent;}
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <class T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}
	void SetPawnData(const UBasePawnData* InPawnData);

	virtual void PostInitializeComponents() override;
protected:

	UPROPERTY(VisibleAnywhere, Category="PLayerState")
	TObjectPtr<UBaseAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category="PLayerState")
	TObjectPtr<const UBasePawnData> PawnData;
};
