#include "ACharacterSetupActor.h"
#include "Core/Structs/FCharacterCustomizationData.h"

ACharacterSetupActor::ACharacterSetupActor()
{
    // Izveidojam Mesh komponenti
    CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh"));
    RootComponent = CharacterMesh; // Padarām to par galveno

    // Šis neļauj tam nokrist cauri zemei, ja ir fizika (pēc izvēles)
    // CharacterMesh->SetCollisionProfileName(TEXT("NoCollision"));
    
    PrimaryActorTick.bCanEverTick = false; // Mums nevajag Tick, lai ietaupītu resursus
    
   // PreviewMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PreviewMesh"));
   //RootComponent = PreviewMesh;
}

void ACharacterSetupActor::UpdatePreview(const FCharacterCustomizationData& Data)
{
    if (!CharacterMesh) return;

    // 1. Dzimuma maiņa
    USkeletalMesh* TargetMesh = Data.bIsMale ? MaleMesh : FemaleMesh;
    if (TargetMesh && CharacterMesh->GetSkeletalMeshAsset() != TargetMesh)
    {
        CharacterMesh->SetSkeletalMesh(TargetMesh);
    }

    // 2. Augums un Svars (Scale)
    // Atceries: X un Y ir platums (Weight), Z ir augstums (Height)
    FVector NewScale = FVector(Data.WeightScale, Data.WeightScale, Data.HeightScale);
    CharacterMesh->SetRelativeScale3D(NewScale);
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

