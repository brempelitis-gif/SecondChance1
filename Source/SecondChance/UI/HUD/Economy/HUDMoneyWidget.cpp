#include "UI/HUD/Economy/HUDMoneyWidget.h"
#include "Components/TextBlock.h"
#include "Player/SecondChancePlayerState.h"

void UHUDMoneyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerState* PS = GetOwningPlayerState())
	{
		if (ASecondChancePlayerState* MyPS = Cast<ASecondChancePlayerState>(PS))
		{
			MyPS->OnMoneyChanged.AddDynamic(this, &UHUDMoneyWidget::HandleMoneyChanged);
			
			// Uzreiz iestatām sākuma vērtību bez animācijas
			TargetMoney = MyPS->GetMoney();
			DisplayMoney = (float)TargetMoney;
			Text_MoneyAmount->SetText(FText::AsCurrency(TargetMoney, TEXT("EUR")));
		}
	}
}

void UHUDMoneyWidget::HandleMoneyChanged(int32 NewAmount)
{
	TargetMoney = NewAmount;
}

void UHUDMoneyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// "Smooth" skaitļu pāreja
	if (!FMath::IsNearlyEqual(DisplayMoney, (float)TargetMoney, 0.1f))
	{
		DisplayMoney = FMath::FInterpTo(DisplayMoney, (float)TargetMoney, InDeltaTime, CountingSpeed);
		
		// Atjaunojam tekstu (noapaļojam uz leju)
		int32 IntToDisplay = FMath::FloorToInt(DisplayMoney);
		Text_MoneyAmount->SetText(FText::AsCurrency(IntToDisplay, TEXT("EUR")));
	}
}