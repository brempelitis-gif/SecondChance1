#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, EPlayerStatType, StatType, float, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SECONDCHANCE_API UPlayerStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerStatsComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	UPROPERTY(BlueprintAssignable, Category = "PlayerStats | Events")
	FOnStatChanged OnStatChanged;

	UFUNCTION(BlueprintPure, Category = "PlayerStats")
	float GetStatValue(EPlayerStatType StatType) const;

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	void ModifyStat(EPlayerStatType StatType, float Amount);

private:
	/** Galvenā funkcija, ko izsauks taimeris */
	void ProcessStatsUpdate();

	UPROPERTY()
	TMap<EPlayerStatType, float> Stats;

	/** Taimera rokturis */
	FTimerHandle StatsUpdateTimerHandle;

	/** Cik bieži atjaunot status (sekundēs) */
	UPROPERTY(EditAnywhere, Category = "PlayerStats | Balance")
	float UpdateFrequency = 1.0f;

	/** Cik punkti tiek atņemti katrā UpdateFrequency ciklā */
	UPROPERTY(EditAnywhere, Category = "PlayerStats | Balance")
	float GlobalDecayRate = 0.5f;
};


