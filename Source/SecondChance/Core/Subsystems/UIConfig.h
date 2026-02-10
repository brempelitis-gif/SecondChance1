#pragma once
#include "Engine/DataAsset.h"
#include "UI/Base/UIBaseWidget.h"
#include "UI/Base/ConfirmationPopUp/UIConfirmationPopup.h"
#include "UIConfig.generated.h"

UCLASS(BlueprintType)
class SECONDCHANCE_API UUIConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// Splash Screen Widget
	UPROPERTY(EditDefaultsOnly, Category = "UI|Classes")
	TSubclassOf<class USplashScreenWidget> SplashWidgetClass;

	// Main Menu Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUIBaseWidget> MainMenuClass;

	// Options Menu Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Classes")
	TSubclassOf<class UUIOptionsMenuBase> OptionsMenuClass;

	// Character Appearance Widget (New Game)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<class UCharacterCreationMain> NewGameWidgetClass;
	
	// Pause Menu Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUIBaseWidget> PauseMenuClass;

	// Confirmation Popup Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Classes|Popups")
	TSubclassOf<UUIConfirmationPopup> ConfirmationPopupClass;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, Category="Audio")
	USoundClass* SFXSoundClass;
	
};