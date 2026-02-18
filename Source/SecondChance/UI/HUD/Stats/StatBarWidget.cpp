#include "UI/HUD/Stats/StatBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Iestatām sākotnējo tekstu, ja tāds ir paredzēts
	if (Text_StatName)
	{
		// Šeit varētu izmantot Enum To String konvertāciju smukākam nosaukumam
		FString StatName = UEnum::GetValueAsString(StatToDisplay);
		StatName.Split(TEXT("::"), nullptr, &StatName); // Noņem "EPlayerStatType::" daļu
		Text_StatName->SetText(FText::FromString(StatName));
	}
}

void UStatBarWidget::InitializeStatBar(EPlayerStatType NewStatType)
{
	StatToDisplay = NewStatType;
}

void UStatBarWidget::UpdateVisuals(float CurrentValue)
{
	if (ProgressBar_Value)
	{
		// Progresa josla Unreal pieņem vērtības no 0.0 līdz 1.0
		ProgressBar_Value->SetPercent(CurrentValue / 100.0f);

		// Krāsu maiņa (pēc izvēles): ja vērtība zema (zem 20%), mainām uz sarkanu
		FLinearColor BarColor = (CurrentValue <= 20.0f) ? FLinearColor::Red : FLinearColor::Green;
		ProgressBar_Value->SetFillColorAndOpacity(BarColor);
	}
}
