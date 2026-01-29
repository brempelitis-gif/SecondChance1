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
int32 UMenuDropdownWidget::GetSelectedIndex() const
{
	return ComboBox ? ComboBox->GetSelectedIndex() : -1;
}




void UMenuDropdownWidget::SetSelectedIndex(int32 Index)
{
	if (ComboBox) ComboBox->SetSelectedIndex(Index);
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



void UMenuDropdownWidget::SetLabel(const FText& InText)
{
	if (Label)
	{
		Label->SetText(InText);
	}
}
void UMenuDropdownWidget::HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	// Pārsūtam notikumu tālāk uz GraphicsWidget
	OnSelectionChanged.Broadcast(SelectedItem, SelectionType);
}






