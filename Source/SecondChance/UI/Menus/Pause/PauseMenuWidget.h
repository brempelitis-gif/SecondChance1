#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UMenuButtonWidget;
class UBackgroundBlur;

UCLASS()
class SECONDCHANCE_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	void BindButtons();
	UPROPERTY(meta = (BindWidgetOptional))
	UBackgroundBlur* BG_Blur; // Blur fons
	
	// Pogas kreisajā malā
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* ResumeBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* SaveBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* LoadBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* OptionsBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* QuitBtn;

	// Teksti pogām (lai varētu viegli lokalizēt vai mainīt bez koda)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pause Menu|Labels")
	FText ResumeBtnLabel = FText::FromString("RESUME");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pause Menu|Labels")
	FText SaveBtnLabel = FText::FromString("SAVE GAME");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pause Menu|Labels")
	FText LoadBtnLabel = FText::FromString("LOAD GAME");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pause Menu|Labels")
	FText OptionsBtnLabel = FText::FromString("OPTIONS");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pause Menu|Labels")
	FText QuitBtnLabel = FText::FromString("MAIN MENU");
	
	// Funkcijas pogām
	UFUNCTION() void HandleResume();
	UFUNCTION() void HandleSave();
	UFUNCTION() void HandleLoad();
	UFUNCTION() void HandleOptions();
	UFUNCTION() void HandleQuit();
};