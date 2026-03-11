#pragma once

#include "CoreMinimal.h"
#include "BasePawnData.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "GameFramework/WorldSettings.h"
#include "Components/PawnComponent.h"
#include "PawnExtensionComponent.generated.h"

class UBasePawnData;
class UBaseAbilitySystemComponent; 
UCLASS()
class SECONDCHANCE_API UPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	static const FName Name_ActorFeatureName;

	virtual FName GetFeatureName() const override { return Name_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;

	void InitializeAbilitySystem(UBaseAbilitySystemComponent* InASC, AActor* InOwner);
	void UnInitializeAbilitySystem();

	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const { return AbilitySystemComponent; }
protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
public:
	void SetupPlayerInputComponent();
	
	static UPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor)
	{
		if (!Actor) return nullptr;
		return Actor->FindComponentByClass<UPawnExtensionComponent>();
	}
	template <class T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}

	void SetPawnData(const UBasePawnData* InPawnData);

	TObjectPtr<const UBasePawnData> PawnData;
	TObjectPtr<UBaseAbilitySystemComponent> AbilitySystemComponent;
};
