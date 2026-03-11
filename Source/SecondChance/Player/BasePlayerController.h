#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UBaseAbilitySystemComponent;
class ABasePlayerState;

class UInputMappingContext;///
class UInputAction;///

UCLASS()
class SECONDCHANCE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent() const;
	ABasePlayerState* GetBasePlayerState() const;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;





	//------------Si dala no veca PlayerControllera, lai varētu izmantot pauzes funkciju----------------
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
