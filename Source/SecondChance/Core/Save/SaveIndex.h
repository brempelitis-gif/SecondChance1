#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/Structs/FSaveMetadata.h" // Iekļauj failu, kur definēji FSaveMetadata
#include "SaveIndex.generated.h"

UCLASS()
class SECONDCHANCE_API USaveIndex : public USaveGame
{
	GENERATED_BODY()

public:
	// Saraksts ar visiem aktīvajiem seiviem
	UPROPERTY(VisibleAnywhere, Category = "SaveSystem")
	TArray<FSaveMetadata> SavedGames;
};
