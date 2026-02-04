#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AGM_MainMenu.generated.h"

UCLASS()
class SECONDCHANCE_API AGM_MainMenu : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGM_MainMenu();

protected:
	virtual void BeginPlay() override;

	// Klase, kuru izmantosim, lai izveidotu Main Menu logrīku (iestatāms BP)
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

	// Glabājam atsauci uz izveidoto logrīku
	UPROPERTY()
	class UUserWidget* CurrentMainMenuWidget;
};
