#include "MenuDropdownWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

void UMenuDropdownWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Apply designer label so it shows in UMG Designer and at runtime if present.
	if (Label)
	{
		Label->SetText(DesignerLabel);
	}

	// If designer options are provided, populate them for preview/runtime defaults.
	if (DesignerOptions.Num() > 0 && ComboBox)
	{
		SetOptions(DesignerOptions);
	}
}

void UMenuDropdownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ComboBox)
	{
		ComboBox->OnSelectionChanged.AddDynamic(
			this, &UMenuDropdownWidget::HandleSelectionChanged
		);
	}
}

void UMenuDropdownWidget::SetOptions(const TArray<FString>& InOptions)
{
	if (!ComboBox) return;

	CachedOptions = InOptions;
	ComboBox->ClearOptions();

	for (const FString& Option : InOptions)
	{
		ComboBox->AddOption(Option);
	}

	if (InOptions.Num() > 0)
	{
		ComboBox->SetSelectedIndex(0);
	}
}

void UMenuDropdownWidget::SetSelectedIndex(int32 Index)
{
	if (!ComboBox) return;
	if (!CachedOptions.IsValidIndex(Index)) return;

	ComboBox->SetSelectedOption(CachedOptions[Index]);
}

int32 UMenuDropdownWidget::GetSelectedIndex() const
{
	if (!ComboBox) return INDEX_NONE;

	const FString Selected = ComboBox->GetSelectedOption();
	return CachedOptions.IndexOfByKey(Selected);
}

void UMenuDropdownWidget::SetLabel(const FText& InText)
{
	if (Label)
	{
		Label->SetText(InText);
	}
}

void UMenuDropdownWidget::HandleSelectionChanged(
	FString SelectedItem,
	ESelectInfo::Type SelectType
)
{
	const int32 Index = CachedOptions.IndexOfByKey(SelectedItem);
	if (Index != INDEX_NONE)
	{
		OnSelectionChanged.Broadcast(Index);
	}
}