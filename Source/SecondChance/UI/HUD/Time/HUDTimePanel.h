#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDTimePanel.generated.h"

class UHUDTimeControlButton;
class UTextBlock;

UCLASS()
class SECONDCHANCE_API UHUDTimePanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Izsauc šo, lai atjaunotu visu pogu vizuālo stāvokli */
	void RefreshButtonHighlights(float CurrentSpeed);
	
protected:
	// BindWidget piesaista pogas no WBP
	UPROPERTY(meta = (BindWidget))
	UHUDTimeControlButton* Btn_Pause;

	UPROPERTY(meta = (BindWidget))
	UHUDTimeControlButton* Btn_Play;

	UPROPERTY(meta = (BindWidget))
	UHUDTimeControlButton* Btn_Fast;

	UPROPERTY(meta = (BindWidget))
	UHUDTimeControlButton* Btn_Ultra;

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_GameTime; // Jāsakrīt ar nosaukumu WBP

	UFUNCTION()
	void UpdateClock(int32 Hours, int32 Minutes);
};
