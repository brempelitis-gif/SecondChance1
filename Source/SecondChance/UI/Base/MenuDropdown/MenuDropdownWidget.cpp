#include "MenuDropdownWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
//#include "SlateCore/Public/Widgets/Input/SComboBox.h" 

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

