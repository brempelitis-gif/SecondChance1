#include "USplashScreenWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USplashScreenWidget::UpdateProgress(float Alpha)
{
	if (LoadingBar)
	{
		// Izmantojam GetPercent() tiešā mainīgā vietā
		float CurrentPercent = LoadingBar->GetPercent();
        
		// FMath::FInterpTo vietā, ja gribi "cietu" progresu bez gludināšanas, 
		// var izmantot vienkārši Alpha. Bet, ja gribi plūstumu:
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float NewPercent = FMath::FInterpTo(CurrentPercent, Alpha, DeltaTime, 5.0f);
        
		// Izmantojam SetPercent() tiešā mainīgā vietā
		LoadingBar->SetPercent(NewPercent);

		if (PercentText)
		{
			// Formatējam tekstu, lai tas vienmēr būtu vesels skaitlis
			int32 IntPercent = FMath::Clamp(FMath::RoundToInt(NewPercent * 100.0f), 0, 100);
			PercentText->SetText(FText::Format(FText::FromString("{0}%"), FText::AsNumber(IntPercent)));
		}
	}
}