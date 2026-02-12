#include "UCharacterCreationMain.h"
#include "Components/WidgetSwitcher.h"
#include "CreationParts/UCharacterSkillTreeWidget.h"
#include "UI/Menus/NewGame/CreationParts/UCharacterAppearanceWidget.h"


void UCharacterCreationMain::NativeConstruct() 
{
	Super::NativeConstruct();

	if (AppearanceStep)
	{
		// Drošības pēc vispirms noņemam, ja nu jau ir piesaistīts, lai nedubultojas
		AppearanceStep->OnNextStepRequested.RemoveDynamic(this, &UCharacterCreationMain::SwitchToSkillTree);
        
		AppearanceStep->OnNextStepRequested.AddDynamic(this, &UCharacterCreationMain::SwitchToSkillTree);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MASTER: AppearanceStep NAV atrasts (null)! Pārbaudi BindWidget nosaukumu Blueprintā."));
	}

	if (SkillTreeStep)
	{
		SkillTreeStep->OnBackStepRequested.RemoveDynamic(this, &UCharacterCreationMain::SwitchToAppearance);
		SkillTreeStep->OnBackStepRequested.AddDynamic(this, &UCharacterCreationMain::SwitchToAppearance);
	}
}
void UCharacterCreationMain::SwitchToSkillTree()
{
	if (StepSwitcher)
	{
		StepSwitcher->SetActiveWidgetIndex(1);
		// Pēc izvēles: paslēpjam 3D aktieri, ja Skill Tree skatā tas traucē
		// PreviewActor->SetActorHiddenInGame(true);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MASTER: StepSwitcher is NULL!"));
	}
}


void UCharacterCreationMain::SwitchToAppearance()
{
	if (StepSwitcher)
	{
		StepSwitcher->SetActiveWidgetIndex(0);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("MASTER: Switching to Index 0 (Appearance)"));
	}
}