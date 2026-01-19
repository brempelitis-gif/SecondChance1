#include "UIOptionsMenuBase.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UUIOptionsMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	BindButtons();

	SetActiveCategory(ESettingsCategory::Audio);
}

void UUIOptionsMenuBase::BindButtons()
{
	if (AudioTab)    AudioTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleAudioTab);
	if (GraphicsTab) GraphicsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGraphicsTab);
	if (ControlsTab) ControlsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleControlsTab);
	if (GameplayTab) GameplayTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGameplayTab);

	if (ApplyButton)  ApplyButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleApply);
	if (CancelButton) CancelButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleCancel);
}

void UUIOptionsMenuBase::SetActiveCategory(ESettingsCategory Category)
{
	if (!CategorySwitcher) return;

	int32 Index = 0;
	switch (Category)
	{
	case ESettingsCategory::Audio:     Index = 0; break;
	case ESettingsCategory::Graphics:  Index = 1; break;
	case ESettingsCategory::Controls:  Index = 2; break;
	case ESettingsCategory::Gameplay:  Index = 3; break;
	default: break;
	}

	CategorySwitcher->SetActiveWidgetIndex(Index);
}

void UUIOptionsMenuBase::HandleAudioTab()
{
	SetActiveCategory(ESettingsCategory::Audio);
}

void UUIOptionsMenuBase::HandleGraphicsTab()
{
	SetActiveCategory(ESettingsCategory::Graphics);
}

void UUIOptionsMenuBase::HandleControlsTab()
{
	SetActiveCategory(ESettingsCategory::Controls);
}

void UUIOptionsMenuBase::HandleGameplayTab()
{
	SetActiveCategory(ESettingsCategory::Gameplay);
}

void UUIOptionsMenuBase::HandleApply()
{
	if (UIManager)
	{
		UIManager->ApplyPendingSettings();
	}
}

void UUIOptionsMenuBase::HandleCancel()
{
	if (UIManager)
	{
		UIManager->CancelPendingSettings();
	}
}

