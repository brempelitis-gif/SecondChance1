#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIConfirmationPopup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPopupResult);
class UTextBlock;
class UMenuButtonWidget;
UCLASS()
class SECONDCHANCE_API UUIConfirmationPopup : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowPopup(FText CategoryName, float Timeout);
	void HidePopup();

	UPROPERTY(BlueprintAssignable)
	FOnPopupResult OnConfirmed;

	UPROPERTY(BlueprintAssignable)
	FOnPopupResult OnTimedOutOrCancelled;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerLabel;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* AcceptButton;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* RevertButton;

private:
	UFUNCTION()
	void HandleAccept();

	UFUNCTION()
	void HandleRevert();

	float CurrentTimeRemaining;
	bool bIsTimerActive = false;
	FText BaseDescriptionText;
};
