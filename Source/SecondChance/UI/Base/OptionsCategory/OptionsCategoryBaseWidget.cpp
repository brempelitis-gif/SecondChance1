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
	// Izmantojam GetTypedOuter, kas ir Unreal iebūvēts un daudz ātrāks veids par manuālu while cilpu
	UUIOptionsMenuBase* ParentOptions = Cast<UUIOptionsMenuBase>(GetTypedOuter<UUIOptionsMenuBase>());
	
	if (ParentOptions)
	{
		// Pārbaudām vai jau neesam piesaistīti
		if (!ParentOptions->OnSettingsChanged.IsAlreadyBound(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged))
		{
			ParentOptions->OnSettingsChanged.AddDynamic(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged);
			UE_LOG(LogTemp, Log, TEXT("Kategorija %d veiksmīgi piesaistīta UUIOptionsMenuBase"), (int32)Category);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Kategorija %d NEVARĒJA atrast tēvu! Pārbaudi WBP hierarhiju."), (int32)Category);
	}
}
void UOptionsCategoryBaseWidget::NativeDestruct()
{
	if (UUIOptionsMenuBase* ParentOptions = Cast<UUIOptionsMenuBase>(GetTypedOuter<UUIOptionsMenuBase>()))
	{
		ParentOptions->OnSettingsChanged.RemoveDynamic(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged);
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

