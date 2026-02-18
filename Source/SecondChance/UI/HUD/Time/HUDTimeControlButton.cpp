#include "UI/HUD/Time/HUDTimeControlButton.h"

#include "HUDTimePanel.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

void UHUDTimeControlButton::NativeConstruct()
{
	Super::NativeConstruct();
	if (Btn_Speed)
	{
		Btn_Speed->OnClicked.AddDynamic(this, &UHUDTimeControlButton::HandleButtonClicked);
	}
	// Sākumā izslēdzam highlight
	SetIsActive(false);
}

void UHUDTimeControlButton::HandleButtonClicked()
{
	// 1. Mainām laika ātrumu pasaulē
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeSpeed);
    
	// 2. Pasakām panelim, ka ātrums ir mainījies, lai tas atjaunotu pogu krāsas
	if (UHUDTimePanel* ParentPanel = Cast<UHUDTimePanel>(GetParent()->GetOuter()->GetOuter())) 
	{
		// Šis ir mazliet "netīrs" veids, labāk ir meklēt caur GetTypedOuter
	}
    
	// Vienkāršāk: Atrodam HUD paneli un izsaucam atjaunošanu
	if (UHUDTimePanel* Panel = GetTypedOuter<UHUDTimePanel>())
	{
		Panel->RefreshButtonHighlights(TimeSpeed);
	}
}
void UHUDTimeControlButton::SetIsActive(bool bActive)
{
	if (Btn_Speed)
	{
		// Mainām pogas fonu (Tint). Ja aktīva - zila/zaļa, ja nē - parasta.
		FLinearColor Color = bActive ? FLinearColor(0.0f, 0.5f, 1.0f, 1.0f) : FLinearColor::White;
		Btn_Speed->SetBackgroundColor(Color);
	}
    
	if (Img_Highlight)
	{
		Img_Highlight->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}