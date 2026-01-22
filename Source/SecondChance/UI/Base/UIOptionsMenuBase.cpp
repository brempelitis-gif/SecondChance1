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
	if (AudioTab)
	{
		AudioTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleAudioTab);
		AudioTab->SetLabel(AudioTabLabel);
	}  
	if (GraphicsTab)
	{
		GraphicsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGraphicsTab);
		GraphicsTab->SetLabel(GraphicsTabLabel);
	}
	if (ControlsTab){
		ControlsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleControlsTab);
		ControlsTab->SetLabel(ControlsTabLabel);
	}
	if (GameplayTab)
	{
		GameplayTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGameplayTab);
		GameplayTab->SetLabel(GameplayTabLabel);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleApply);
		ApplyButton->SetLabel(ApplyButtonLabel);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleCancel);
		CancelButton->SetLabel(CancelButtonLabel);
	}
}

void UUIOptionsMenuBase::SetActiveCategory(ESettingsCategory Category)
{
	UE_LOG(LogTemp, Warning, TEXT("Switcer start"));
	if (!CategorySwitcher) return;
	UE_LOG(LogTemp, Warning, TEXT("Switcher Found"));
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
	UE_LOG(LogTemp, Warning, TEXT("Handle Audio"));
	SetActiveCategory(ESettingsCategory::Audio);
}

void UUIOptionsMenuBase::HandleGraphicsTab()
{
	UE_LOG(LogTemp, Warning, TEXT("Handle Graphics"));
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

