#pragma once

#include "Core/Structs/FCharacterCustomizationData.h"
#include "ACharacterPreviewActor.generated.h"


UCLASS()
class SECONDCHANCE_API ACharacterPreviewActor : public AActor
{
	GENERATED_BODY()

public:
	ACharacterPreviewActor();

	// Šo funkciju izsauks Widget slaideri
	UFUNCTION(BlueprintCallable, Category = "Customization")
	void UpdatePreview(const FCharacterCustomizationData& Data);
	void RotatePreview(float DeltaYaw);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* CharacterMesh; // Placeholder kapsulai

	// Materiāls, lai mainītu krāsu (Skin color)
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
};
