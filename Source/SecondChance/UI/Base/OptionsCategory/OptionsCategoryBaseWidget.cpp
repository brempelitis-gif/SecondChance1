#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

void UOptionsCategoryBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 1. Atrodam UIManager (tavs esošais kods)
	if (UGameInstance* GI = GetGameInstance())
	{
		UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
	}

	// 2. Atrodam OptionsBaseWidget, ejot uz augšu pa hierarhiju
	UOptionsBaseWidget* ParentOptions = nullptr;
	UWidget* CurrentParent = GetParent();
    
	while (CurrentParent)
	{
		ParentOptions = Cast<UOptionsBaseWidget>(CurrentParent);
		if (ParentOptions) break;
        
		CurrentParent = CurrentParent->GetParent();
	}

	// 3. Piesaistāmies delegātam
	if (ParentOptions)
	{
		ParentOptions->OnSettingsChanged.AddDynamic(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged);
		UE_LOG(LogTemp, Log, TEXT("Category %d successfully bound to OptionsBaseWidget"), (int32)Category);
	}
}

void UOptionsCategoryBaseWidget::NativeDestruct()
{
	/*
	if (UIManager)
	{
		UIManager->OnSettingsChanged.RemoveDynamic(
			this,
			&UOptionsCategoryBaseWidget::HandleSettingsChanged
		);
	}*/

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
	UOptionsBaseWidget* ParentOptions = nullptr;
	UWidget* CurrentParent = GetParent();
    
	while (CurrentParent)
	{
		ParentOptions = Cast<UOptionsBaseWidget>(CurrentParent);
		if (ParentOptions) break;
		CurrentParent = CurrentParent->GetParent();
	}

	return ParentOptions && ParentOptions->IsCategoryPending(Category);
}

