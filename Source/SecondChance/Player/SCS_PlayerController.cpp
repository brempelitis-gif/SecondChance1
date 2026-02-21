#include "SCS_PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyGameInstance.h"

void ASCS_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Ja nākam no citas kartes, uzspiežam fokusu uz spēli
	FInputModeGameOnly GameMode;
	SetInputMode(GameMode);
	bShowMouseCursor = false;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Notīrām vecos contextus (drošībai) un pievienojam jauno
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ASCS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Piesaistām funkciju IA_Pause darbībai
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASCS_PlayerController::HandlePause);
	}
}

void ASCS_PlayerController::HandlePause()
{
	UE_LOG(LogTemp, Warning, TEXT("PAUZE PIEPRASĪTA!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Poga nospiesta!"));
	// Tā pati loģika, ko uzrakstījām iepriekš
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->TogglePauseMenu();
	}
}