#pragma once

#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

UCLASS()
class SECONDCHANCE_API USaveGameData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	FDateTime SaveDate;

	UPROPERTY(BlueprintReadOnly)
	int32 SlotIndex;

	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> ScreenshotData;
};