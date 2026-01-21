#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString PlayerName;

    // Simple representation for character initial choices. Can be changed to enums or structs later.
    UPROPERTY(BlueprintReadWrite)
    FString Gender;

    UPROPERTY(BlueprintReadWrite)
    float Weight = 70.f;

    UPROPERTY(BlueprintReadWrite)
    float Height = 170.f;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor SkinColor = FLinearColor::White;
};

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

    // Store initial character settings used when creating this save
    UPROPERTY(BlueprintReadOnly)
    FCharacterData CharacterData;
};
