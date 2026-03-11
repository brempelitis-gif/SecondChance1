#include "BaseGameCharacter.h"
#include "PawnExtensionComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BaseCharacterMovementComponent.h"


ABaseGameCharacter::ABaseGameCharacter(const FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UBaseCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	MovementComponent = Cast<UBaseCharacterMovementComponent>(GetCharacterMovement());
	PawnExtComponent = CreateDefaultSubobject<UPawnExtensionComponent>(TEXT("PawnExt"));
}

void ABaseGameCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}else if (MovementComponent->IsMovingOnGround())
	{
		Crouch();
	}
}

void ABaseGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Šeit pievieno savas kustību kontroles (MoveForward, MoveRight utt.)
}












void ABaseGameCharacter::ApplyCustomizationData(const FCharacterCustomizationData& Data)
{
	// Saglabājam datus lokāli
	CurrentCustomizationData = Data;

	// Izsaucam vizuālo atjaunināšanu
	UpdateVisuals();

	UE_LOG(LogTemp, Log, TEXT("SCharacter: Dati sekmīgi pielietoti spēlētājam %s"), *Data.PlayerName);
}

void ABaseGameCharacter::UpdateVisuals()
{
	// 1. Mērogs (Height un Weight)
	// Izmantojam WeightScale priekš X un Y, un HeightScale priekš Z (augstuma)
	FVector NewScale = FVector(
		CurrentCustomizationData.WeightScale, 
		CurrentCustomizationData.WeightScale, 
		CurrentCustomizationData.HeightScale
	);
	SetActorScale3D(NewScale);

	// 2. Dzimuma materiālu maiņa
	if (GetMesh())
	{
		UMaterialInterface* SelectedMaterial = CurrentCustomizationData.bIsMale ? MaleMaterial : FemaleMaterial;
		
		if (SelectedMaterial)
		{
			GetMesh()->SetMaterial(0, SelectedMaterial);
		}
	}

	// 3. Statu pielietošana (piemērs)
	// Šeit tu vēlāk vari pieslēgt savu AttributeSystem vai vienkārši mainīgos
	// float BaseHealth = 100.0f + (CurrentCustomizationData.Strength * 10.0f);
}
