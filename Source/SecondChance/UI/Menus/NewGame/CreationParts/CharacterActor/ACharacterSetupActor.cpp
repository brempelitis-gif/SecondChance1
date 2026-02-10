#include "ACharacterSetupActor.h"
#include "Core/Structs/FCharacterCustomizationData.h"

ACharacterSetupActor::ACharacterSetupActor()
{
    PrimaryActorTick.bCanEverTick = false; // Mums nevajag Tick, lai ietaupītu resursus
    
    PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
    RootComponent = PreviewMesh;
}

void ACharacterSetupActor::UpdatePreview(const FCharacterCustomizationData& CurrentData)
{
    // 1. DZIMUMS: Pārslēdzam materiālu
    UpdateGenderVisuals(CurrentData.bIsMale);

    // 2. AUGUMS (Z ass): Aprēķinām mērogu
    // Vīriešiem (bIsMale=true) skala 1.0 - 1.2
    // Sievietēm (bIsMale=false) skala 0.9 - 1.1
    float MinH = CurrentData.bIsMale ? 1.0f : 0.9f;
    float MaxH = CurrentData.bIsMale ? 1.2f : 1.1f;
    float NewScaleZ = FMath::Lerp(MinH, MaxH, CurrentData.HeightScale);

    // 3. SVARS (X un Y asis): Padarām tēlu platāku vai šaurāku
    // 0.8 ir ļoti tievs, 1.3 ir dūšīgs
    float NewScaleXY = FMath::Lerp(0.8f, 1.3f, CurrentData.WeightScale);

    // Uzstādām jauno izmēru visām asīm uzreiz
    SetActorScale3D(FVector(NewScaleXY, NewScaleXY, NewScaleZ));

    // 4. KRĀSA: Nosūtām krāsu uz materiālu
    UpdateSkinColor(CurrentData.SkinColor);
}

void ACharacterSetupActor::UpdateGenderVisuals(bool bIsMale)
{
    UMaterialInterface* TargetMat = bIsMale ? MaleMaterial : FemaleMaterial;

    if (TargetMat && PreviewMesh)
    {
        // Izveidojam dinamisku materiālu, lai varētu mainīt tā parametrus (BodyColor)
        CurrentDynamicMaterial = PreviewMesh->CreateAndSetMaterialInstanceDynamic(0);
        if (CurrentDynamicMaterial)
        {
            CurrentDynamicMaterial->CopyMaterialUniformParameters(TargetMat);
        }
    }
}

void ACharacterSetupActor::UpdateSkinColor(FLinearColor NewColor)
{
    if (CurrentDynamicMaterial)
    {
        // "BodyColor" ir parametra nosaukums tavam materiālam Unreal Editorā
        CurrentDynamicMaterial->SetVectorParameterValue(FName("BodyColor"), NewColor);
    }
}

void ACharacterSetupActor::RotateCharacter(float Value)
{
    // Griežam tēlu ap Z asi (Value varētu būt teiksim 10.0f vai -10.0f)
    AddActorLocalRotation(FRotator(0.0f, Value, 0.0f));
}

