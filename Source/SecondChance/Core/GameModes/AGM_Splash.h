#pragma once

#include "GameFramework/GameModeBase.h"
#include "AGM_Splash.generated.h"

UCLASS()
class SECONDCHANCE_API AGM_Splash : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_Splash();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SplashWidgetClass;

	UPROPERTY()
	class USplashScreenWidget* SplashWidget;

	FTimerHandle ProgressTimerHandle;
	FName NextLevelName = TEXT("/Game/ManaSpele/Levels/L_MainMenu");

	void StartAsyncLoad();
	void UpdateLoadingProgress(); // Funkcija, ko izsauks taimeris
};