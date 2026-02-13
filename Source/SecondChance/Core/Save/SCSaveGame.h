#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/Structs/FCharacterCustomizationData.h" // Obligāti iekļauj savu struktūru!
#include "SCSaveGame.generated.h"

/**
 * Šī klase ir faila struktūra, kas tiks saglabāta uz diska (.sav fails)
 */
UCLASS()
class SECONDCHANCE_API USCSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 1. Spēlētāja izskats un statuss (tava struktūra)
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FCharacterCustomizationData PlayerData;

	// 2. Slota nosaukums (lai zinātu, kuru failu pārrakstīt)
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FString SaveSlotName;

	// 3. Datums (lai zinātu, kurš ir jaunākais/vecākais)
	UPROPERTY(VisibleAnywhere, Category = "SaveData")
	FDateTime CreationDate;

	// 4. Ekrānšāviņa ceļš (ja vēlies to glabāt šeit, bet mēs to ģenerējam no vārda)
	// UPROPERTY(VisibleAnywhere, Category = "SaveData")
	// FString ScreenshotPath;
};
