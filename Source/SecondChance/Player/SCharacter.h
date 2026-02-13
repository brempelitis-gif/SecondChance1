#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Structs/FCharacterCustomizationData.h"
#include "SCharacter.generated.h"

UCLASS()
class SECONDCHANCE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
	virtual void BeginPlay() override;

	/** Funkcija, kas fiziski pielāgo tēla vizuālo izskatu pasaulē */
	void UpdateVisuals();

	/** Materiāli, kurus spēle izmantos atkarībā no dzimuma */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	class UMaterialInterface* MaleMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	class UMaterialInterface* FemaleMaterial;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Publiski pieejama funkcija, ko izsauks GameMode līmeņa sākumā */
	void ApplyCustomizationData(const FCharacterCustomizationData& Data);

private:
	/** Pašreizējie dati, kas saņemti no GameInstance */
	FCharacterCustomizationData CurrentCustomizationData;
};
