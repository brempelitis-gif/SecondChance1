#include "ACharacterSetupActor.h"

ACharacterSetupActor::ACharacterSetupActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
	RootComponent = PreviewMesh;
}

void ACharacterSetupActor::UpdateGenderVisuals(bool bIsFemale)
{
	if (bIsFemale && FemaleMaterial)
	{
		PreviewMesh->SetMaterial(0, FemaleMaterial);
	}
	else if (!bIsFemale && MaleMaterial)
	{
		PreviewMesh->SetMaterial(0, MaleMaterial);
	}
}

void ACharacterSetupActor::UpdateHeight(float NormalizedHeight, bool bIsFemale)
{
	// Piemērs: Vīrieši 170-200cm, Sievietes 160-190cm
	float MinHeight = bIsFemale ? 0.9f : 1.0f; // Mērogs
	float MaxHeight = bIsFemale ? 1.1f : 1.2f;

	float NewScaleZ = FMath::Lerp(MinHeight, MaxHeight, NormalizedHeight);
    
	// Mainām tikai augstumu (Z asi) vai visu skalu
	SetActorScale3D(FVector(1.0f, 1.0f, NewScaleZ));
}

void ACharacterSetupActor::RotateCharacter(float Value)
{
	// Griežam ap Z asi
	AddActorLocalRotation(FRotator(0.0f, Value * 2.0f, 0.0f));
}

void ACharacterSetupActor::UpdatePreview(const FCharacterCustomizationData& CurrentData)
{
}
