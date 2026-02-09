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
	// Shows the popup with the given category name and timeout duration.
	void ShowPopup(FText CategoryName, float Timeout);
	void HidePopup();

	UPROPERTY(BlueprintAssignable)
	FOnPopupResult OnConfirmed;

	UPROPERTY(BlueprintAssignable)
	FOnPopupResult OnTimedOutOrCancelled;

protected:
	virtual void NativePreConstruct() override;
	void NativeConstruct();
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerLabel;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* AcceptButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText AcceptButtonLabel = FText::FromString("Accept");

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* RevertButton;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText RevertButtonLabel = FText::FromString("Revert");

private:
	UFUNCTION()
	void HandleAccept();

	UFUNCTION()
	void HandleRevert();

	// Taimera funkcija, ko sauksim katru sekundi
	void UpdateTimerTick();

	FTimerHandle CountdownTimerHandle;
	int32 SecondsRemaining;
	FText CachedCategoryName;

	void RefreshDescriptionText();
};
