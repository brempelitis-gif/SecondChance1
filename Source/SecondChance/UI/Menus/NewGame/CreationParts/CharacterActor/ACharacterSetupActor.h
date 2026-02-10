#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UI/Menus/NewGame/CreationParts/UCharacterAppearanceWidget.h"
#include "ACharacterSetupActor.generated.h"

UCLASS()
class SECONDCHANCE_API ACharacterSetupActor : public AActor
{
	GENERATED_BODY()
    
public:    
	ACharacterSetupActor();

	// Vizuālais elements (Kapsula vai Mesh)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visuals")
	UStaticMeshComponent* PreviewMesh;

	// Materiāli krāsu maiņai (Zils/Sarkans)
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UMaterialInterface* MaleMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UMaterialInterface* FemaleMaterial;

	// Funkcijas, ko izsauks UI
	UFUNCTION(BlueprintCallable)
	void UpdateGenderVisuals(bool bIsFemale);

	UFUNCTION(BlueprintCallable)
	void UpdateHeight(float NormalizedHeight, bool bIsFemale); // 0.0 līdz 1.0

	UFUNCTION(BlueprintCallable)
	void RotateCharacter(float Value); // -1 (pa kreisi) vai +1 (pa labi)
	void UpdatePreview(const FCharacterCustomizationData& CurrentData);
};
