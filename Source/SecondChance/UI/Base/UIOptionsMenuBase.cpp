#include "UIOptionsMenuBase.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UUIOptionsMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	BindButtons();
	if (UIManager)
	{
		UIManager->OnSettingsChanged.AddDynamic(this, &UUIOptionsMenuBase::HandleSettingsChanged);
	}
	UpdateActionButtonsVisibility();
}
void UUIOptionsMenuBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (AudioTab)
	{
		AudioTab->SetLabel(AudioTabLabel);
	}  
	if (GraphicsTab)
	{
		GraphicsTab->SetLabel(GraphicsTabLabel);
	}
	if (ControlsTab){
		ControlsTab->SetLabel(ControlsTabLabel);
	}
	if (GameplayTab)
	{
		GameplayTab->SetLabel(GameplayTabLabel);
	}
	if (ApplyButton)
	{
		ApplyButton->SetLabel(ApplyButtonLabel);
	}
	if (CancelButton)
	{
		CancelButton->SetLabel(CancelButtonLabel);
	}
	// At this point BindWidget children should be initialized.
	// Set default category (safe to call now).
	SetActiveCategory(ESettingsCategory::Audio);
}
void UUIOptionsMenuBase::BindButtons()
{
	if (AudioTab)
	{
		AudioTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleAudioTab);
	}  
	if (GraphicsTab)
	{
		GraphicsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGraphicsTab);
	}
	if (ControlsTab){
		ControlsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleControlsTab);
	}
	if (GameplayTab)
	{
		GameplayTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGameplayTab);
	}

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleApply);
	}
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleCancel);
	}
}

void UUIOptionsMenuBase::SetActiveCategory(ESettingsCategory Category) const
{
	if (!CategorySwitcher)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUIOptionsMenuBase::SetActiveCategory - CategorySwitcher is null"));
		return;
	}
	int32 Index = 0;
	switch (Category)
	{
	case ESettingsCategory::Audio:     Index = 0; break;
	case ESettingsCategory::Graphics:  Index = 1; break;
	case ESettingsCategory::Controls:  Index = 2; break;
	case ESettingsCategory::Gameplay:  Index = 3; break;
	default: break;
	}
	const int32 Num = CategorySwitcher->GetNumWidgets();

	if (Index < 0 || Index >= Num)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveCategory - Index %d is out of bounds (NumWidgets = %d)"), Index, Num);
		return;
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
	if (!UIManager)
	{
		return;;
	}
	if (UIManager->IsCategoryPending(ESettingsCategory::Graphics))
	{
		UIManager->OnConfirmSettingsRequired.Broadcast(ESettingsCategory::Graphics);
	}
	else
	{
		UIManager->ApplyPendingSettings();
		UpdateActionButtonsVisibility();
	}
}

void UUIOptionsMenuBase::HandleCancel()
{
	if (UIManager)
	{
		UIManager->CancelPendingSettings();
	}
	//After cancelling, hide buttons until further changes are made
	UpdateActionButtonsVisibility();
}
void UUIOptionsMenuBase::HandleSettingsChanged(ESettingsCategory Category)
{
	UpdateActionButtonsVisibility();
}
void UUIOptionsMenuBase::UpdateActionButtonsVisibility()
{
	if (!ApplyButton||!CancelButton)
	{
		return;
	}
	const bool bHasAudioPending    = UIManager ? UIManager->IsCategoryPending(ESettingsCategory::Audio) : false;
	const bool bHasGraphicsPending    = UIManager ? UIManager->IsCategoryPending(ESettingsCategory::Graphics) : false;
	const bool bShouldShow  = bHasAudioPending || bHasGraphicsPending;
	CancelButton->SetVisibility((bShouldShow) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ApplyButton->SetVisibility((bShouldShow) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
