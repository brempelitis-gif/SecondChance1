#include "ACharacterPreviewActor.h"

ACharacterPreviewActor::ACharacterPreviewActor()
{
}

void ACharacterPreviewActor::UpdatePreview(const FCharacterCustomizationData& Data)
{
	if (!CharacterMesh) return;

	// 1. Dzimuma vizualizācija (Zila vs Sarkana kapsula)
	FLinearColor DisplayColor = Data.bIsMale ? FLinearColor::Blue : FLinearColor::Red;
	
	// Ja tev vēl nav DynamicMaterial, izveidojam to
	if (!DynamicMaterial) {
		DynamicMaterial = CharacterMesh->CreateDynamicMaterialInstance(0);
	}
	
	if (DynamicMaterial) {
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), DisplayColor);
		// Šeit varētu likt arī Data.SkinColor, kad būs īstie meshi
	}

	// 2. Auguma un svara vizualizācija caur Scale
	// Piemēram: Vīrieši vidēji 1.0, Sievietes 0.9 + slaidera ietekme
	float BaseHeight = Data.bIsMale ? 1.0f : 0.92f;
	float FinalHeight = BaseHeight + (Data.HeightScale * 0.2f); // Slaideris dod +-10cm
	
	float FinalWidth = 1.0f + (Data.WeightScale * 0.3f);

	CharacterMesh->SetRelativeScale3D(FVector(FinalWidth, FinalWidth, FinalHeight));
}
// Actora pusē:
void ACharacterPreviewActor::RotatePreview(float DeltaYaw)
{
	AddActorLocalRotation(FRotator(0.0f, DeltaYaw, 0.0f));
}
