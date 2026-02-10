#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACharacterSetupActor.generated.h"

// Forward declaration, lai izvairītos no Circular Dependency
struct FCharacterCustomizationData;

UCLASS()
class SECONDCHANCE_API ACharacterSetupActor : public AActor
{
	GENERATED_BODY()
    
public:    
	ACharacterSetupActor();

	// Galvenais modelis (Kapsula vai Mesh)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	UStaticMeshComponent* PreviewMesh;

	// Materiālu sagataves, ko iestatīsi Unreal Editorā
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UMaterialInterface* MaleMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UMaterialInterface* FemaleMaterial;

	// Funkcija, ko izsauks tavs UI Widget
	UFUNCTION(BlueprintCallable, Category = "Character Customization")
	void UpdatePreview(const FCharacterCustomizationData& CurrentData);

	// Funkcija tēla pagriešanai
	UFUNCTION(BlueprintCallable, Category = "Character Customization")
	void RotateCharacter(float Value);

	void UpdateGenderVisuals(bool bIsMale);
private:
	// Mainīgais, kas ļauj mums mainīt krāsu reāllaikā
	UPROPERTY()
	UMaterialInstanceDynamic* CurrentDynamicMaterial;

	
	void UpdateSkinColor(FLinearColor NewColor);
};