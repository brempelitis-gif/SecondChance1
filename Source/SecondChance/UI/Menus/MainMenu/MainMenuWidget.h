#pragma once

#include "CoreMinimal.h"
#include "UI/Base/UIBaseWidget.h"
#include "MainMenuWidget.generated.h"

class UMenuButtonWidget;
class UUIManagerSubsystem;

UCLASS()
class SECONDCHANCE_API UMainMenuWidget : public UUIBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	/* =======================
	 * POGAS (BindWidget)
	 * ======================= */
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* ContinueButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText ContinueLabel = FText::FromString("Continue");

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* NewGameButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText NewGameLabel = FText::FromString("New Game");

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* OptionsButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText OptionsLabel = FText::FromString("Options");

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* QuitButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText QuitLabel = FText::FromString("Quit");

	/* =======================
	 * KLASES OPCIJU LOGAM
	 * ======================= */
	UPROPERTY(EditDefaultsOnly, Category = "Main Menu|Navigation")
	TSubclassOf<UUserWidget> OptionsMenuClass;

private:
	/** Handleri pogu klikšķiem */
	UFUNCTION() void HandleContinueClicked();
	UFUNCTION() void HandleNewGameClicked();
	UFUNCTION() void HandleOptionsClicked();
	UFUNCTION() void HandleQuitClicked();

	void BindButtons();
};
