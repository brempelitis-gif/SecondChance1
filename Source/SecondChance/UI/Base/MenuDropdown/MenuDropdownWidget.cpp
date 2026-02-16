#include "MenuDropdownWidget.h"
#include "Components/ComboBoxString.h"

void UMenuDropdownWidget::NativePreConstruct()
{
	// 1. Šis izpildīs UBaseLabeledWidget loģiku (uzliks LabelText no LabelDefaultValue)
	Super::NativePreConstruct();

	// 2. Aizpildām ComboBox ar opcijām no Designer loga (ja tādas ir)
	if (ComboBox && DesignerOptions.Num() > 0)
	{
		ComboBox->ClearOptions();
		for (const FString& Option : DesignerOptions)
		{
			ComboBox->AddOption(Option);
		}
		// Uzstādām pirmo kā noklusējumu, ja nekas nav izvēlēts
		if (ComboBox->GetSelectedIndex() == -1)
		{
			ComboBox->SetSelectedIndex(0);
		}
	}
}

void UMenuDropdownWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ComboBox)
	{
		ComboBox->OnSelectionChanged.AddDynamic(this, &UMenuDropdownWidget::HandleSelectionChanged);
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

int32 UMenuDropdownWidget::GetSelectedIndex() const
{
	return ComboBox ? ComboBox->GetSelectedIndex() : -1;
}

void UMenuDropdownWidget::SetSelectedIndex(int32 Index)
{
	if (ComboBox) ComboBox->SetSelectedIndex(Index);
}

void UMenuDropdownWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	OnSelectionChanged.Broadcast(SelectedItem, SelectionType);
}

void UMenuDropdownWidget::ClearOptions()
{
	if (ComboBox)
	{
		ComboBox->ClearOptions();
		CachedOptions.Empty();
	}
}

void UMenuDropdownWidget::AddOption(const FString& Option) const
{
	if (ComboBox) ComboBox->AddOption(Option);
}