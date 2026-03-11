#include "BasePlayerController.h"
#include "Character/BasePlayerState.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MyGameInstance.h"

UBaseAbilitySystemComponent* ABasePlayerController::GetBaseAbilitySystemComponent() const
{
	const ABasePlayerState* PS= GetBasePlayerState();
	return PS ? PS->GetBaseAbilitySystemComponent() : nullptr;
}

ABasePlayerState* ABasePlayerController::GetBasePlayerState() const
{
	return CastChecked<ABasePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

void ABasePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UBaseAbilitySystemComponent* ASC = GetBaseAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}












void ABasePlayerController::BeginPlay()
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

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Piesaistām funkciju IA_Pause darbībai
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABasePlayerController::HandlePause);
	}
}

void ABasePlayerController::HandlePause()
{
	UE_LOG(LogTemp, Warning, TEXT("PAUZE PIEPRASĪTA!"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Poga nospiesta!"));
	// Tā pati loģika, ko uzrakstījām iepriekš
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->TogglePauseMenu();
	}
}