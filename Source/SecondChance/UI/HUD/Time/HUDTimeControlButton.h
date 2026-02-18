#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDTimeControlButton.generated.h"

class UButton;
class UImage;

UCLASS()
class SECONDCHANCE_API UHUDTimeControlButton : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Iestata pogas ātruma vērtību (0=pauze, 1=normāls, utt.) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Time")
	float TimeSpeed = 1.0f;

	/** Funkcija, ko izsauksim no Paneļa, lai pateiktu pogai: "Tu esi aktīva!" */
	void SetIsActive(bool bActive);

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Speed;

	// Pievienojam bildi vai borderi, ko iekrāsot, kad poga ir aktīva
	UPROPERTY(meta = (BindWidgetOptional))
	UImage* Img_Highlight;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleButtonClicked();
};
