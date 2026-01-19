#include "OptionsBaseWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"

void UOptionsBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UGameInstance* GI = GetGameInstance())
	{
		UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
	}

	if (!UIManager)
	{
		UE_LOG(LogTemp, Error, TEXT("OptionsBaseWidget: UIManagerSubsystem not found"));
		return;
	}

	// Bind buttons
	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UOptionsBaseWidget::HandleApplyClicked);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UOptionsBaseWidget::HandleCancelClicked);
	}

	// Listen for settings changes
	UIManager->OnSettingsChanged.AddDynamic(this, &UOptionsBaseWidget::OnSettingsChanged);

	RefreshUI();
}

void UOptionsBaseWidget::NativeDestruct()
{
	if (UIManager)
	{
		UIManager->OnSettingsChanged.RemoveDynamic(this, &UOptionsBaseWidget::OnSettingsChanged);
	}

	Super::NativeDestruct();
}

void UOptionsBaseWidget::HandleApplyClicked()
{
	if (UIManager)
	{
		UIManager->ApplyPendingSettings();
	}
}

void UOptionsBaseWidget::HandleCancelClicked()
{
	if (UIManager)
	{
		UIManager->CancelPendingSettings();
	}
}

void UOptionsBaseWidget::OnSettingsChanged(ESettingsCategory ChangedCategory)
{
	if (ChangedCategory == Category || ChangedCategory == ESettingsCategory::None)
	{
		RefreshUI();
	}
}

void UOptionsBaseWidget::RefreshUI()
{
	if (!UIManager)
	{
		return;
	}

	const bool bHasPending = UIManager->IsCategoryPending(Category);

	if (ApplyButton)
	{
		ApplyButton->SetIsEnabled(bHasPending);
	}

	if (CancelButton)
	{
		CancelButton->SetIsEnabled(bHasPending);
	}
}

