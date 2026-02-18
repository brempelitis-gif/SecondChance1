#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDMoneyWidget.generated.h"

class UTextBlock;

UCLASS()
class SECONDCHANCE_API UHUDMoneyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_MoneyAmount;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void HandleMoneyChanged(int32 NewAmount);

private:
	float DisplayMoney = 0.0f;
	int32 TargetMoney = 0;

	/** Cik ātri skaitļi mainās (lielāks skaitlis = ātrāk) */
	UPROPERTY(EditAnywhere, Category = "Economy")
	float CountingSpeed = 5.0f;
};