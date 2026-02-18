#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameTimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeChanged, int32, Hours, int32, Minutes);

UCLASS()
class SECONDCHANCE_API UGameTimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Taimera startēšana, kad apakšsistēma sāk darbu
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable, Category = "Time")
	FOnTimeChanged OnTimeChanged;

	UFUNCTION(BlueprintPure, Category = "Time")
	FText GetFormattedTime() const;

private:
	/** Galvenā funkcija, ko izsauks taimeris */
	void AdvanceTime();

	FTimerHandle TimeUpdateHandle;
	
	int32 CurrentHours = 8;
	int32 CurrentMinutes = 0;

	// Cik reālas sekundes ilgst viena spēles minūte
	const float GameMinuteDuration = 1.0f; 
};