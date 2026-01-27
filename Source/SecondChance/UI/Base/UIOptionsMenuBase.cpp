#include "UIOptionsMenuBase.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/ConfirmationPopUp/UIConfirmationPopup.h"

void UUIOptionsMenuBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UIManager = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	
	// Jauns: Izveidojam Popup vienu reizi un paslēpjam
	if (PopupClass && !CachedPopup)
	{
		CachedPopup = CreateWidget<UUIConfirmationPopup>(this, PopupClass);
		CachedPopup->AddToViewport(100);
		CachedPopup->HidePopup();
	}
	
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
	// Uzstādām tekstus Editora priekšskatījumam
	if (AudioTab) AudioTab->SetLabel(AudioTabLabel);
	if (GraphicsTab) GraphicsTab->SetLabel(GraphicsTabLabel);
	if (ControlsTab) ControlsTab->SetLabel(ControlsTabLabel);
	if (GameplayTab) GameplayTab->SetLabel(GameplayTabLabel);
	if (ApplyButton) ApplyButton->SetLabel(ApplyButtonLabel);
	if (CancelButton) CancelButton->SetLabel(CancelButtonLabel);
	// At this point BindWidget children should be initialized.
	// Set default category (safe to call now).
	SetActiveCategory(ESettingsCategory::Audio);
}
void UUIOptionsMenuBase::BindButtons()
{
	if (AudioTab) AudioTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleAudioTab);
	if (GraphicsTab) GraphicsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGraphicsTab);
	if (ControlsTab) ControlsTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleControlsTab);
	if (GameplayTab) GameplayTab->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleGameplayTab);

	if (ApplyButton) ApplyButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleApply);
	if (CancelButton) CancelButton->OnClicked.AddDynamic(this, &UUIOptionsMenuBase::HandleCancel);
}

void UUIOptionsMenuBase::SetActiveCategory(ESettingsCategory Category)
{
	CurrentCategory = Category;
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
	// Jauns noteikums: Pārslēdzot kategoriju, pogas pazūd, ja šajā kategorijā nav izmaiņu
	UpdateActionButtonsVisibility();
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
	if (!UIManager) return;

	if (CurrentCategory == ESettingsCategory::Graphics && CachedPopup)
	{
		// Piesaistām popupa notikumus UIManager sistēmai
		CachedPopup->OnConfirmed.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::ApplyPendingSettings);
		CachedPopup->OnTimedOutOrCancelled.AddUniqueDynamic(UIManager, &UUIManagerSubsystem::CancelPendingSettings);
		
		// Atjaunojam UI pēc popupa slēgšanas
		CachedPopup->OnConfirmed.AddUniqueDynamic(this, &UUIOptionsMenuBase::UpdateActionButtonsVisibility);
		CachedPopup->OnTimedOutOrCancelled.AddUniqueDynamic(this, &UUIOptionsMenuBase::UpdateActionButtonsVisibility);

		CachedPopup->ShowPopup(GraphicsTabLabel, 15.0f);
	}
	else
	{
		// Audio/Gameplay iestatījumus apstiprinām uzreiz
		UIManager->ApplyPendingSettings();
		UpdateActionButtonsVisibility();
	}
}

void UUIOptionsMenuBase::HandleCancel()
{
	if (UIManager) UIManager->CancelPendingSettings();
	//After cancelling, hide buttons until further changes are made
	UpdateActionButtonsVisibility();
}
void UUIOptionsMenuBase::HandleSettingsChanged(ESettingsCategory Category)
{
	UpdateActionButtonsVisibility();
}
void UUIOptionsMenuBase::UpdateActionButtonsVisibility()
{
	if (!ApplyButton || !CancelButton || !UIManager) return;

	// Pārbauda tikai aktuālo sadaļu
	bool bShow = UIManager->IsCategoryPending(CurrentCategory);
	
	ESlateVisibility Vis = bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	ApplyButton->SetVisibility(Vis);
	CancelButton->SetVisibility(Vis);
}
