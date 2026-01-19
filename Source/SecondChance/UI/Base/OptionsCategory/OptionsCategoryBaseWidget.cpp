#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "Kismet/GameplayStatics.h"

void UOptionsCategoryBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UGameInstance* GI = GetGameInstance())
	{
		UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
	}

	if (UIManager)
	{
		UIManager->OnSettingsChanged.AddDynamic(
			this,
			&UOptionsCategoryBaseWidget::HandleSettingsChanged
		);
	}
}

void UOptionsCategoryBaseWidget::NativeDestruct()
{
	if (UIManager)
	{
		UIManager->OnSettingsChanged.RemoveDynamic(
			this,
			&UOptionsCategoryBaseWidget::HandleSettingsChanged
		);
	}

	Super::NativeDestruct();
}

void UOptionsCategoryBaseWidget::HandleSettingsChanged(ESettingsCategory ChangedCategory)
{
	// Ja mainījās cita kategorija → ignorējam
	if (ChangedCategory != Category && ChangedCategory != ESettingsCategory::None)
	{
		return;
	}

	// Šeit nekas vizuāls netiek darīts
	// Child widgeti var override Blueprint Event
}

bool UOptionsCategoryBaseWidget::IsPending() const
{
	return UIManager && UIManager->IsCategoryPending(Category);
}

