#include "UCharacterCreationMain.h"
#include "Components/WidgetSwitcher.h"
#include "CreationParts/USkillTreeWidget.h"
#include "UI/Menus/NewGame/CreationParts/UCharacterAppearanceWidget.h"

void UCharacterCreationMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Šeit mēs sasaistām bērnu logrīku pogas ar Master loģiku
	// Tev bērnu klasēs vajadzēs publiskus delegātus (Events)
	if (AppearanceStep)
	{
		// Kad Appearance logā nospiež Next
		AppearanceStep->OnNextStepRequested.AddDynamic(this, &UCharacterCreationMain::SwitchToSkillTree);
	}

	if (SkillTreeStep)
	{
		// Kad SkillTree logā nospiež Back
		SkillTreeStep->OnBackStepRequested.AddDynamic(this, &UCharacterCreationMain::SwitchToAppearance);
	}
}

void UCharacterCreationMain::SwitchToSkillTree()
{
	if (StepSwitcher) StepSwitcher->SetActiveWidgetIndex(1);
}

void UCharacterCreationMain::SwitchToAppearance()
{
	if (StepSwitcher) StepSwitcher->SetActiveWidgetIndex(0);
}