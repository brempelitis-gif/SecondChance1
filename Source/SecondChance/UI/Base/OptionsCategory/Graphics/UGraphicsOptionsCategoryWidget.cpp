
#include "UGraphicsOptionsCategoryWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"

void UGraphicsOptionsCategoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Uzstādām tekstus Editora priekšskatījumam
	if (ResolutionCombo) ResolutionCombo->SetLabel(ResolutionComboLabel);
	if (QualityCombo) QualityCombo->SetLabel(QualityComboLabel);
	if (WindowModeCombo) WindowModeCombo->SetLabel(WindowModeComboLabel);
	if (VSyncCheckBox) VSyncCheckBox->SetLabel(VSyncCheckBoxLabel);
	if (ResolutionScaleSlider) ResolutionScaleSlider->SetLabel(ResolutionScaleLabel);
}

void UGraphicsOptionsCategoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Piesaistām visus eventus
	if (ResolutionCombo) ResolutionCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionChanged);
	if (QualityCombo) QualityCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleQualityChanged);
	if (WindowModeCombo) WindowModeCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleWindowModeChanged);
	if (VSyncCheckBox) VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleVSyncChanged);
	if (ResolutionScaleSlider) ResolutionScaleSlider->OnValueChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionScaleChanged);

	if (UIManager)
	{
		UIManager->OnSettingsChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::RefreshUIFromCurrentSettings);
	}
}


void UGraphicsOptionsCategoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PopulateComboBoxes();
	RefreshUIFromCurrentSettings(ESettingsCategory::Graphics);
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

void UGraphicsOptionsCategoryWidget::PopulateComboBoxes()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;

	// 1. Rezolūcijas
	ResolutionCombo->ClearOptions();
	ResolutionsArray.Empty();
	FScreenResolutionArray ScreenResolutions;
	if (RHIGetAvailableResolutions(ScreenResolutions, true))
	{
		for (const FScreenResolutionRHI& Res : ScreenResolutions)
		{
			FIntPoint ResPoint(Res.Width, Res.Height);
			if (!ResolutionsArray.Contains(ResPoint))
			{
				ResolutionsArray.Add(ResPoint);
				ResolutionCombo->AddOption(FString::Printf(TEXT("%d x %d"), Res.Width, Res.Height));
			}
		}
	}

	// 2. Kvalitāte
	QualityCombo->ClearOptions();
	for (const FString& L : QualityLabels) QualityCombo->AddOption(L);

	// 3. Window Mode
	WindowModeCombo->ClearOptions();
	for (const FString& L : WindowModeLabels) WindowModeCombo->AddOption(L);
}

void UGraphicsOptionsCategoryWidget::RefreshUIFromCurrentSettings(ESettingsCategory ChangedCategory)
{
	if (ChangedCategory != ESettingsCategory::Graphics) return;
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;

	// Atjaunojam Rezolūciju
	int32 ResIndex = ResolutionsArray.Find(Settings->GetScreenResolution());
	if (ResolutionCombo && ResIndex != INDEX_NONE) ResolutionCombo->SetSelectedIndex(ResIndex);

	// Atjaunojam Kvalitāti
	if (QualityCombo) QualityCombo->SetSelectedIndex(Settings->GetOverallScalabilityLevel());

	// Atjaunojam Window Mode
	if (WindowModeCombo) WindowModeCombo->SetSelectedIndex((int32)Settings->GetFullscreenMode());

	// Atjaunojam VSync
	if (VSyncCheckBox) VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());

	// Atjaunojam Resolution Scale (0.0 - 1.0 diapazonā spēlei, slider parasti 0-100 vai 0-1)
	if (ResolutionScaleSlider) ResolutionScaleSlider->SetValue(Settings->GetResolutionScaleNormalized());
	
	// Atjaunojam VSync izmantojot jauno klasi
	if (VSyncCheckBox) VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());
}
// --- Eventu realizācija ---

void UGraphicsOptionsCategoryWidget::HandleResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Direct) return;
	int32 SelectedIndex = ResolutionCombo->GetSelectedIndex();
	if (ResolutionsArray.IsValidIndex(SelectedIndex)&& UIManager)
	{
		if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
		{
			Settings->SetScreenResolution(ResolutionsArray[SelectedIndex]);
			UIManager->MarkCategoryPending(ESettingsCategory::Graphics);
		}
	}
}

void UGraphicsOptionsCategoryWidget::HandleQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Direct) return;
	int32 SelectedIndex = QualityCombo->GetSelectedIndex();
	if (QualityLabels.IsValidIndex(SelectedIndex)&& UIManager)
	{
		if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
		{
			Settings->SetOverallScalabilityLevel(SelectedIndex);
			UIManager->MarkCategoryPending(ESettingsCategory::Graphics);
		}
	}
}

void UGraphicsOptionsCategoryWidget::HandleWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Direct) return;
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings && UIManager)
	{
		// EWindowMode: 0 = Fullscreen, 1 = WindowedFullscreen, 2 = Windowed
		Settings->SetFullscreenMode((EWindowMode::Type)WindowModeCombo->GetSelectedIndex());
		UIManager->MarkCategoryPending(ESettingsCategory::Graphics);
	}
}

void UGraphicsOptionsCategoryWidget::HandleVSyncChanged(bool bIsChecked)
{
	if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
	{
		Settings->SetVSyncEnabled(bIsChecked);
		if (UIManager) UIManager->MarkCategoryPending(ESettingsCategory::Graphics);
	}
}

void UGraphicsOptionsCategoryWidget::HandleResolutionScaleChanged(float Value)
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (Settings && UIManager)
	{
		// Iestatām "Resolution Scale" (piem. 0.7 = 70% no natīvās rezolūcijas)
		Settings->SetResolutionScaleNormalized(Value);
		UIManager->MarkCategoryPending(ESettingsCategory::Graphics);
	}
}
