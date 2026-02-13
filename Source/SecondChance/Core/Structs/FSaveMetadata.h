#pragma once

#include "CoreMinimal.h"
#include "FSaveMetadata.generated.h"

USTRUCT(BlueprintType)
struct FSaveMetadata
{
	GENERATED_BODY()

	// Unikāls faila nosaukums uz diska (piem. "SaveSlot_01")
	UPROPERTY(BlueprintReadOnly, Category = "SaveSystem")
	FString SlotName;

	// Spēlētāja redzamais vārds
	UPROPERTY(BlueprintReadOnly, Category = "SaveSystem")
	FString PlayerName;

	// Kad spēle saglabāta (lai zinātu, kurš ir vecākais)
	UPROPERTY(BlueprintReadOnly, Category = "SaveSystem")
	FDateTime SaveDate;

	// Noklusējuma konstruktors
	FSaveMetadata()
	{
		SaveDate = FDateTime::Now();
	}
};