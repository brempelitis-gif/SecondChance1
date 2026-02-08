#include "UI/Base/OptionsCategory/Graphics/UGraphicsOptionsCategoryWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h"
#include "UI/Base/UIOptionsMenuBase.h" // Svarīgi: Īstais headeris

void UGraphicsOptionsCategoryWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (ResolutionCombo) ResolutionCombo->SetLabel(ResolutionComboLabel);
    if (QualityCombo) QualityCombo->SetLabel(QualityComboLabel);
    if (WindowModeCombo) WindowModeCombo->SetLabel(WindowModeComboLabel);
    if (VSyncCheckBox) VSyncCheckBox->SetLabel(VSyncCheckBoxLabel);
    if (ResolutionScaleSlider) ResolutionScaleSlider->SetLabel(ResolutionScaleLabel);
}

void UGraphicsOptionsCategoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ResolutionCombo) ResolutionCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionChanged);
    if (QualityCombo) QualityCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleQualityChanged);
    if (WindowModeCombo) WindowModeCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleWindowModeChanged);
    if (VSyncCheckBox) VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleVSyncChanged);
    if (ResolutionScaleSlider) ResolutionScaleSlider->OnValueChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionScaleChanged);
}

void UGraphicsOptionsCategoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PopulateComboBoxes();
	// Tāpat kā Audio, dodam mazu brīdi ielādei
	FTimerHandle RefreshHandle;
	GetWorld()->GetTimerManager().SetTimer(RefreshHandle, this, &UGraphicsOptionsCategoryWidget::RefreshUIFromCurrentSettings, 0.1f, false);
}

UUIOptionsMenuBase* UGraphicsOptionsCategoryWidget::GetParentOptions() const
{
	return Cast<UUIOptionsMenuBase>(GetTypedOuter<UUIOptionsMenuBase>());
}

void UGraphicsOptionsCategoryWidget::PopulateComboBoxes()
{
	if (ResolutionCombo)
	{
		ResolutionCombo->ClearOptions();
		ResolutionsArray.Empty();
        
		FScreenResolutionArray ScreenResolutions;
		if (RHIGetAvailableResolutions(ScreenResolutions, true))
		{
			// Sakārtojam rezolūcijas no lielākās uz mazāko (ērtāk lietotājam)
			for (int32 i = ScreenResolutions.Num() - 1; i >= 0; --i)
			{
				FIntPoint ResPoint(ScreenResolutions[i].Width, ScreenResolutions[i].Height);
				if (!ResolutionsArray.Contains(ResPoint))
				{
					ResolutionsArray.Add(ResPoint);
					ResolutionCombo->AddOption(FString::Printf(TEXT("%d x %d"), ResPoint.X, ResPoint.Y));
				}
			}
		}
	}

    if (QualityCombo)
    {
        QualityCombo->ClearOptions();
        for (const FString& L : QualityLabels) QualityCombo->AddOption(L);
    }

    if (WindowModeCombo)
    {
        WindowModeCombo->ClearOptions();
        for (const FString& L : WindowModeLabels) WindowModeCombo->AddOption(L);
    }
}

void UGraphicsOptionsCategoryWidget::HandleSettingsChanged(ESettingsCategory ChangedCategory)
{
    if (ChangedCategory == ESettingsCategory::Graphics || ChangedCategory == ESettingsCategory::None)
    {
        RefreshUIFromCurrentSettings();
    }
}

void UGraphicsOptionsCategoryWidget::RefreshUIFromCurrentSettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;

	bIsRefreshing = true; // Nobloķējam, lai dropdownu maiņa neizsauktu Apply atkal

	// 1. Rezolūcija
	if (ResolutionCombo)
	{
		int32 ResIndex = ResolutionsArray.Find(Settings->GetScreenResolution());
		if (ResIndex != INDEX_NONE) 
		{
			ResolutionCombo->SetSelectedIndex(ResIndex);
		}
	}

	// 2. Kvalitāte (Overall Scalability)
	if (QualityCombo) 
	{
		int32 Quality = Settings->GetOverallScalabilityLevel();
		QualityCombo->SetSelectedIndex(Quality);
	}

	// 3. Loga režīms
	if (WindowModeCombo) 
	{
		WindowModeCombo->SetSelectedIndex((int32)Settings->GetFullscreenMode());
	}

	// 4. VSync
	if (VSyncCheckBox) 
	{
		VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());
	}

	// 5. Resolution Scale
	if (ResolutionScaleSlider) 
	{
		// Normalized vērtība ir 0.0 - 1.0 (vai 10-100 atkarībā no versijas)
		ResolutionScaleSlider->SetValue(Settings->GetResolutionScaleNormalized());
	}

	bIsRefreshing = false;
	UE_LOG(LogTemp, Log, TEXT("Graphics UI atsvaidzināts no GameUserSettings."));
}

void UGraphicsOptionsCategoryWidget::HandleResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (bIsRefreshing || SelectionType == ESelectInfo::Direct) return;
    
	int32 Index = ResolutionCombo->GetSelectedIndex();
	if (ResolutionsArray.IsValidIndex(Index))
	{
		if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
		{
			Settings->SetScreenResolution(ResolutionsArray[Index]);
			if (UUIOptionsMenuBase* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
		}
	}
}

void UGraphicsOptionsCategoryWidget::HandleQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (bIsRefreshing || SelectionType == ESelectInfo::Direct) return;
    
    int32 SelectedIndex = QualityCombo->GetSelectedIndex();
    if (QualityLabels.IsValidIndex(SelectedIndex))
    {
       if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
       {
          Settings->SetOverallScalabilityLevel(SelectedIndex);
          if (UUIOptionsMenuBase* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
       }
    }
}

void UGraphicsOptionsCategoryWidget::HandleWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (bIsRefreshing || SelectionType == ESelectInfo::Direct) return;
    
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
       Settings->SetFullscreenMode((EWindowMode::Type)WindowModeCombo->GetSelectedIndex());
       if (UUIOptionsMenuBase* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
    }
}

void UGraphicsOptionsCategoryWidget::HandleVSyncChanged(bool bIsChecked)
{
    if (bIsRefreshing) return;
    
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
       Settings->SetVSyncEnabled(bIsChecked);
       if (UUIOptionsMenuBase* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
    }
}

void UGraphicsOptionsCategoryWidget::HandleResolutionScaleChanged(float Value)
{
    if (bIsRefreshing) return;
    
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
       Settings->SetResolutionScaleNormalized(Value);
       if (UUIOptionsMenuBase* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
    }
}
