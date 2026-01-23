#include "UGraphicsOptionsCategoryWidget.h"
#include "Components/ComboBoxString.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"

void UGraphicsOptionsCategoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Example default options so the UMG Designer shows them.
	// Designers can override these in the UMenuDropdownWidget's DesignerOptions or in the UMG Blueprint.
	if (ResolutionCombo)
	{
		TArray<FString> Resolutions = {
			TEXT("1920x1080"),
			TEXT("1600x900"),
			TEXT("1280x720"),
		};
		ResolutionCombo->SetOptions(Resolutions);
		ResolutionCombo->SetLabel(FText::FromString(TEXT("Resolution")));
	}

	if (WindowModeCombo)
	{
		TArray<FString> Modes = {
			TEXT("Fullscreen"),
			TEXT("Windowed"),
			TEXT("Borderless")
		};
		WindowModeCombo->SetOptions(Modes);
		WindowModeCombo->SetLabel(FText::FromString(TEXT("Window Mode")));
	}

	if (QualityCombo)
	{
		TArray<FString> Qualities = {
			TEXT("Low"),
			TEXT("Medium"),
			TEXT("High"),
			TEXT("Ultra")
		};
		QualityCombo->SetOptions(Qualities);
		QualityCombo->SetLabel(FText::FromString(TEXT("Quality")));
	}
}

void UGraphicsOptionsCategoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Cache UIManager
	if (UGameInstance* GI = GetGameInstance())
	{
		UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
	}

	// Bind to dropdown events (UMenuDropdownWidget::OnSelectionChanged broadcasts int32)
	if (ResolutionCombo)
	{
		ResolutionCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionChanged);
		// set initial selection from UIManager if available
		if (UIManager)
		{
			ResolutionCombo->SetSelectedIndex(UIManager->GetPendingResolutionIndex());
		}
	}

	if (WindowModeCombo)
	{
		WindowModeCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleWindowModeChanged);
		if (UIManager)
		{
			WindowModeCombo->SetSelectedIndex(UIManager->GetPendingWindowMode());
		}
	}

	if (QualityCombo)
	{
		QualityCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleQualityChanged);
		if (UIManager)
		{
			QualityCombo->SetSelectedIndex(UIManager->GetPendingQuality());
		}
	}
}

void UGraphicsOptionsCategoryWidget::NativeDestruct()
{
	// Unbind to avoid dangling references
	if (ResolutionCombo)
	{
		ResolutionCombo->OnSelectionChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionChanged);
	}
	if (WindowModeCombo)
	{
		WindowModeCombo->OnSelectionChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::HandleWindowModeChanged);
	}
	if (QualityCombo)
	{
		QualityCombo->OnSelectionChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::HandleQualityChanged);
	}

	Super::NativeDestruct();
}

/* === Handlers === */

void UGraphicsOptionsCategoryWidget::HandleResolutionChanged(int32 SelectedIndex)
{
	if (UIManager)
	{
		UIManager->SetResolution(SelectedIndex);
	}
}

void UGraphicsOptionsCategoryWidget::HandleWindowModeChanged(int32 SelectedIndex)
{
	if (UIManager)
	{
		UIManager->SetWindowMode(SelectedIndex);
	}
}

void UGraphicsOptionsCategoryWidget::HandleQualityChanged(int32 SelectedIndex)
{
	if (UIManager)
	{
		UIManager->SetGraphicsQuality(SelectedIndex);
	}
}