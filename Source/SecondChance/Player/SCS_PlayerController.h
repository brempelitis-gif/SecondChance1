#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h" // Nepieciešams Enhanced Input
#include "SCS_PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SECONDCHANCE_API ASCS_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Aseti, ko piesaistīsim Blueprintā
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	// Funkcija pauzei
	void HandlePause();
};