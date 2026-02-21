#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UMenuButtonWidget;

UCLASS()
class SECONDCHANCE_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

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

	// Funkcijas pogām
	UFUNCTION() void HandleResume();
	UFUNCTION() void HandleSave();
	UFUNCTION() void HandleLoad();
	UFUNCTION() void HandleOptions();
	UFUNCTION() void HandleQuit();
};