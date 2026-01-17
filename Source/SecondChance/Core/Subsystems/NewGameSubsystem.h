#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "NewGameSubsystem.generated.h"

UCLASS()
class SECONDCHANCE_API UNewGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartNewGame();

	UFUNCTION(BlueprintCallable)
	void ConfirmCharacter(const FString& PlayerName);

private:
	FString PendingPlayerName;
};
