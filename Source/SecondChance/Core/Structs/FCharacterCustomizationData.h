#pragma once

#include "CoreMinimal.h"
#include "FCharacterCustomizationData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterCustomizationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName = TEXT("Spēlētājs");

	UPROPERTY(BlueprintReadWrite)
	bool bIsMale = true;

	UPROPERTY(BlueprintReadWrite)
	float HeightScale = 1.0f; // Slider 0.0 - 1.0, ko vēlāk pārvēršam mērogā

	UPROPERTY(BlueprintReadWrite)
	float WeightScale = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	int32 Age = 25;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 SkillPoints = 10; // Sākuma punkti

	UPROPERTY(BlueprintReadWrite)
	FLinearColor SkinColor = FLinearColor::White;
};
