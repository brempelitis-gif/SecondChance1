#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UCharacterCreationMain.generated.h"

UCLASS()
class SECONDCHANCE_API UCharacterCreationMain : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	// Galvenais komponents logu pārslēgšanai
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* StepSwitcher;

	// Atsauces uz bērnu logrīkiem
	UPROPERTY(meta = (BindWidget))
	class UCharacterAppearanceWidget* AppearanceStep;

	UPROPERTY(meta = (BindWidget))
	class USkillTreeWidget* SkillTreeStep; // Pieņemot, ka tev būs šāda klase

	// Funkcijas, ko izsauks bērnu logrīki caur delegātiem
	UFUNCTION() void SwitchToSkillTree();
	UFUNCTION() void SwitchToAppearance();
};
