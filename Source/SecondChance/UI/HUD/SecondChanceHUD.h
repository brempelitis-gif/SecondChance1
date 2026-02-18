#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SecondChanceHUD.generated.h"

class UMainHUDWidget;

UCLASS()
class SECONDCHANCE_API ASecondChanceHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** Ļauj mums no C++ piekļūt HUD logrīkam, ja vajag ko mainīt lidojumā */
	UFUNCTION(BlueprintPure, Category = "HUD")
	UMainHUDWidget* GetMainHUD() const { return MainHUD; }

protected:
	virtual void BeginPlay() override;

	/** Blueprint klasē mēs norādīsim, kuru WBP_MainHUD izmantot */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UMainHUDWidget> MainHUDClass;

private:
	/** Pašreizējā HUD logrīka instance ekrānā */
	UPROPERTY()
	UMainHUDWidget* MainHUD;
};
