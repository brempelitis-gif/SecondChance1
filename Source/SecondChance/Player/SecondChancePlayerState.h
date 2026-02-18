#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SecondChancePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewAmount);

UCLASS()
class SECONDCHANCE_API ASecondChancePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASecondChancePlayerState();

	UFUNCTION(BlueprintPure, Category = "Economy")
	int32 GetMoney() const { return CurrentMoney; }

	UFUNCTION(BlueprintCallable, Category = "Economy")
	void AddMoney(int32 Amount);

	UPROPERTY(BlueprintAssignable, Category = "Economy")
	FOnMoneyChanged OnMoneyChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Economy")
	int32 CurrentMoney = 1000; // Sākuma kapitāls
};
