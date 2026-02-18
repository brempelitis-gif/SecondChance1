#include "UI/HUD/Time/HUDTimePanel.h"

#include "Components/TextBlock.h"
#include "Core/Time/GameTimeSubsystem.h"
#include "UI/HUD/Time/HUDTimeControlButton.h"

void UHUDTimePanel::NativeConstruct()
{
	Super::NativeConstruct();

	// Piešķiram katrai pogai tās ātrumu
	if (Btn_Pause) Btn_Pause->TimeSpeed = 0.0f;
	if (Btn_Play)  Btn_Play->TimeSpeed  = 1.0f;
	if (Btn_Fast)  Btn_Fast->TimeSpeed  = 2.0f;
	if (Btn_Ultra) Btn_Ultra->TimeSpeed = 4.0f;

	// Pieslēdzamies laika signālam
	if (UGameTimeSubsystem* TimeSub = GetGameInstance()->GetSubsystem<UGameTimeSubsystem>())
	{
		TimeSub->OnTimeChanged.AddDynamic(this, &UHUDTimePanel::UpdateClock);
		
		// Manuāli izsaucam update uzreiz, lai rādītu sākuma laiku (piem. 08:00)
		// Nevis gaidām pirmo "tikšķi"
		UpdateClock(8, 0); 
	}
	
	// Sākumā iezīmējam Play pogu (jo laiks iet uz 1.0)
	RefreshButtonHighlights(1.0f);
}

void UHUDTimePanel::UpdateClock(int32 Hours, int32 Minutes)
{
	if (Text_GameTime)
	{
		// Formatējam uz 00:00 stilu
		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Hours, Minutes);
		Text_GameTime->SetText(FText::FromString(TimeString));
	}
}
void UHUDTimePanel::RefreshButtonHighlights(float CurrentSpeed)
{
	TArray<UHUDTimeControlButton*> Buttons;
	Buttons.Add(Btn_Pause);
	Buttons.Add(Btn_Play);
	Buttons.Add(Btn_Fast);
	Buttons.Add(Btn_Ultra);

	for (UHUDTimeControlButton* B : Buttons)
	{
		if (B)
		{
			// Ja pogas ātrums sakrīt ar pašreizējo - iezīmējam
			B->SetIsActive(FMath::IsNearlyEqual(B->TimeSpeed, CurrentSpeed));
		}
	}
}
