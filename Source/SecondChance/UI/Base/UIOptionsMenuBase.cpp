#include "UIOptionsMenuBase.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UUIOptionsMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	BindButtons();
}
void UUIOptionsMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	// At this point BindWidget children should be initialized.
	// Set default category (safe to call now).
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
	UE_LOG(LogTemp, Log, TEXT("SetActiveCategory: Category=%d -> Index=%d, SwitcherNum=%d, CurrentIndex=%d"),
		   (int32)Category, Index, Num, CategorySwitcher->GetActiveWidgetIndex());

	if (Index < 0 || Index >= Num)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetActiveCategory - Index %d is out of bounds (NumWidgets = %d)"), Index, Num);
		return;
	}

	UWidget* TargetWidget = CategorySwitcher->GetWidgetAtIndex(Index);
	if (TargetWidget)
	{
		UE_LOG(LogTemp, Log, TEXT("SetActiveCategory - Activating widget at index %d: %s"), Index, *TargetWidget->GetName());
	}
	CategorySwitcher->SetActiveWidgetIndex(Index);
	// Confirm it actually changed
	UE_LOG(LogTemp, Log, TEXT("After SetActiveWidgetIndex: ActiveIndex=%d"), CategorySwitcher->GetActiveWidgetIndex());

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

