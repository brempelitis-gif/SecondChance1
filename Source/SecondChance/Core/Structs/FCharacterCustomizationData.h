#pragma once

#include "CoreMinimal.h"
#include "FCharacterCustomizationData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterCustomizationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PlayerName = "Player"; // Noklusējuma vārds

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsMale = true; // Sākam kā vīrietis

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeightScale = 1.0f; // Slider 0.0 - 1.0, ko vēlāk pārvēršam mērogā

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeightScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Age = 25;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 SkillPoints = 10; // Sākuma punkti

	UPROPERTY(BlueprintReadWrite)
	FLinearColor SkinColor = FLinearColor::White;
};
