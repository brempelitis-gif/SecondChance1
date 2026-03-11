#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Core/Structs/FCharacterCustomizationData.h"
#include "BaseGameCharacter.generated.h"

class UBaseCharacterMovementComponent;
class UPawnExtensionComponent;

UCLASS()
class SECONDCHANCE_API ABaseGameCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ABaseGameCharacter(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterMovement)
	UBaseCharacterMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CharacterMovement)
	TObjectPtr<UPawnExtensionComponent> PawnExtComponent;
	void ToggleCrouch();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;








//public:
//	ASCharacter();

protected:
//	virtual void BeginPlay() override;

	/** Funkcija, kas fiziski pielāgo tēla vizuālo izskatu pasaulē */
	void UpdateVisuals();

	/** Materiāli, kurus spēle izmantos atkarībā no dzimuma */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	class UMaterialInterface* MaleMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	class UMaterialInterface* FemaleMaterial;

public:	
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Publiski pieejama funkcija, ko izsauks GameMode līmeņa sākumā */
	void ApplyCustomizationData(const FCharacterCustomizationData& Data);

private:
	/** Pašreizējie dati, kas saņemti no GameInstance */
	FCharacterCustomizationData CurrentCustomizationData;
};

