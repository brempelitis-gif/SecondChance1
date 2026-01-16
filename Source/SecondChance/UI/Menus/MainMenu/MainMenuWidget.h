#pragma once

#include "UI/Base/UIBaseWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UScrollBox;
class USaveSlotWidget;
class USaveGameSubsystem;

UCLASS()
class SECONDCHANCE_API UMainMenuWidget : public UUIBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void RefreshSaveList();

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnQuitClicked();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* NewGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SaveListBox;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveSlotWidget> SaveSlotWidgetClass;
};