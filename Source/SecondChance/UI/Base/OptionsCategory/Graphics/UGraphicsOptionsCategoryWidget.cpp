#include "UGraphicsOptionsCategoryWidget.h"
#include "Components/ComboBoxString.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"

void UGraphicsOptionsCategoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Ensure UIManager is available (OptionsCategoryBaseWidget usually sets this)
	if (!UIManager)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
		}
	}

	// Populate options
	PopulateCombos();

	// Bind delegates
	if (ResolutionCombo)
	{
		ResolutionCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::OnResolutionSelected);
	}
	if (WindowModeCombo)
	{
		WindowModeCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::OnWindowModeSelected);
	}
	if (QualityCombo)
	{
		QualityCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::OnQualitySelected);
	}

	if (UIManager)
	{
		UIManager->OnSettingsChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleManagerSettingsChanged);
	}

	// Initialize selections from pending values
	RefreshFromManager();
}

void UGraphicsOptionsCategoryWidget::NativeDestruct()
{
	if (ResolutionCombo)
	{
		ResolutionCombo->OnSelectionChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::OnResolutionSelected);
	}
	if (WindowModeCombo)
	{
		WindowModeCombo->OnSelectionChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::OnWindowModeSelected);
	}
	if (QualityCombo)
	{
		QualityCombo->OnSelectionChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::OnQualitySelected);
	}
	if (UIManager)
	{
		UIManager->OnSettingsChanged.RemoveDynamic(this, &UGraphicsOptionsCategoryWidget::HandleManagerSettingsChanged);
	}

	Super::NativeDestruct();
}

void UGraphicsOptionsCategoryWidget::PopulateCombos()
{
	// Clear caches and existing options
	CachedResolutionStrings.Empty();
	if (ResolutionCombo) ResolutionCombo->ClearOptions();
	if (WindowModeCombo) WindowModeCombo->ClearOptions();
	if (QualityCombo) QualityCombo->ClearOptions();

	// Resolutions from UIManager (UIManager->SupportedResolutions is public)
	if (UIManager)
	{
		for (const FIntPoint& R : UIManager->SupportedResolutions)
		{
			const FString Str = FString::Printf(TEXT("%d x %d"), R.X, R.Y);
			CachedResolutionStrings.Add(Str);
			if (ResolutionCombo) ResolutionCombo->AddOption(Str);
		}
	}

	// Window mode options - indices must match UIManager expectation (0=Fullscreen,1=Windowed,2=WindowedFullscreen)
	if (WindowModeCombo)
	{
		WindowModeCombo->AddOption(TEXT("Fullscreen"));
		WindowModeCombo->AddOption(TEXT("Windowed"));
		WindowModeCombo->AddOption(TEXT("Windowed Fullscreen"));
	}

	// Quality options - index mapping 0..3 (adjust labels as you prefer)
	if (QualityCombo)
	{
		QualityCombo->AddOption(TEXT("Low"));
		QualityCombo->AddOption(TEXT("Medium"));
		QualityCombo->AddOption(TEXT("High"));
		QualityCombo->AddOption(TEXT("Epic"));
	}
}

void UGraphicsOptionsCategoryWidget::RefreshFromManager()
{
	if (!UIManager) return;

	bSuppressEvents = true;

	// Resolution
	if (ResolutionCombo)
	{
		int32 Index = UIManager->GetPendingResolutionIndex();
		if (CachedResolutionStrings.IsValidIndex(Index))
		{
			ResolutionCombo->SetSelectedOption(CachedResolutionStrings[Index]);
		}
		else if (CachedResolutionStrings.Num() > 0)
		{
			// Fallback
			ResolutionCombo->SetSelectedOption(CachedResolutionStrings[0]);
		}
	}

	// Window mode
	if (WindowModeCombo)
	{
		int32 ModeIndex = UIManager->GetPendingWindowMode();
		// Clamp to valid range (0..2)
		if (ModeIndex < 0) ModeIndex = 0;
		if (ModeIndex > 2) ModeIndex = 2;
		// Map index to option text
		const TArray<FString> WindowModes = { TEXT("Fullscreen"), TEXT("Windowed"), TEXT("Windowed Fullscreen") };
		WindowModeCombo->SetSelectedOption(WindowModes.IsValidIndex(ModeIndex) ? WindowModes[ModeIndex] : WindowModes[0]);
	}

	// Quality
	if (QualityCombo)
	{
		int32 QualityIndex = UIManager->GetPendingQuality();
		if (QualityIndex < 0) QualityIndex = 0;
		if (QualityIndex > 3) QualityIndex = 3;
		const TArray<FString> QualityNames = { TEXT("Low"), TEXT("Medium"), TEXT("High"), TEXT("Epic") };
		QualityCombo->SetSelectedOption(QualityNames.IsValidIndex(QualityIndex) ? QualityNames[QualityIndex] : QualityNames[0]);
	}

	bSuppressEvents = false;
}

void UGraphicsOptionsCategoryWidget::OnResolutionSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSuppressEvents || !UIManager) return;

	// Find index in cached strings
	int32 FoundIndex = INDEX_NONE;
	for (int32 i = 0; i < CachedResolutionStrings.Num(); ++i)
	{
		if (CachedResolutionStrings[i] == SelectedItem)
		{
			FoundIndex = i;
			break;
		}
	}

	if (FoundIndex != INDEX_NONE)
	{
		UIManager->SetResolution(FoundIndex);
	}
}

void UGraphicsOptionsCategoryWidget::OnWindowModeSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSuppressEvents || !UIManager) return;

	int32 ModeIndex = 0;
	if (SelectedItem == TEXT("Fullscreen")) ModeIndex = 0;
	else if (SelectedItem == TEXT("Windowed")) ModeIndex = 1;
	else ModeIndex = 2; // Windowed Fullscreen

	UIManager->SetWindowMode(ModeIndex);
}

void UGraphicsOptionsCategoryWidget::OnQualitySelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bSuppressEvents || !UIManager) return;

	int32 QualityIndex = 0;
	if (SelectedItem == TEXT("Low")) QualityIndex = 0;
	else if (SelectedItem == TEXT("Medium")) QualityIndex = 1;
	else if (SelectedItem == TEXT("High")) QualityIndex = 2;
	else QualityIndex = 3; // Epic

	UIManager->SetGraphicsQuality(QualityIndex);
}

void UGraphicsOptionsCategoryWidget::HandleManagerSettingsChanged(ESettingsCategory ChangedCategory)
{
	if (ChangedCategory == Category || ChangedCategory == ESettingsCategory::None)
	{
		RefreshFromManager();
	}
}
