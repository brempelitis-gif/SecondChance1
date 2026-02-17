#include "UI/Menus/Options/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h" // Svarīgi: Iekļaujam jauno bāzi
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Blueprint/WidgetTree.h"

void UOptionsCategoryBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UGameInstance* GI = GetGameInstance())
	{
		UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
	}
	// Meklējam tēvu izmantojot GetTypedOuter - tas atrod pirmo OptionsBaseWidget hierarhijā virs šī
	if (UOptionsBaseWidget* ParentOptions = GetTypedOuter<UOptionsBaseWidget>())
	{
		if (!ParentOptions->OnSettingsChanged.IsAlreadyBound(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged))
		{
			ParentOptions->OnSettingsChanged.AddDynamic(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged);
		}
	}
}

void UOptionsCategoryBaseWidget::NativeDestruct()
{
	if (UOptionsBaseWidget* ParentOptions = GetTypedOuter<UOptionsBaseWidget>())
	{
		ParentOptions->OnSettingsChanged.RemoveDynamic(this, &UOptionsCategoryBaseWidget::HandleSettingsChanged);
	}
	Super::NativeDestruct();
}

void UOptionsCategoryBaseWidget::HandleSettingsChanged(ESettingsCategory ChangedCategory)
{
	// Ja ChangedCategory ir None, tas nozīmē "atsvaidzināt visu"
	if (ChangedCategory != Category && ChangedCategory != ESettingsCategory::None)
	{
		return;
	}
    
	// Šeit var pievienot BP eventu, ja nepieciešams vizuāli reaģēt uzreiz
}

bool UOptionsCategoryBaseWidget::IsPending() const
{
	// Izmantojam to pašu GetTypedOuter metodi arī šeit
	if (UOptionsBaseWidget* ParentOptions = GetTypedOuter<UOptionsBaseWidget>())
	{
		return ParentOptions->IsCategoryPending(Category);
	}

	return false;
}
