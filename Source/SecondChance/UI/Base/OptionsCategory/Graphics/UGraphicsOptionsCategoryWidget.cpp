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
    
    // Neliela aizkave, lai nodrošinātu, ka viss ir gatavs
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
    // Ja mainījās grafika vai nekas (Cancel/Revert gadījumā), atsvaidzinām
    if (ChangedCategory == ESettingsCategory::Graphics || ChangedCategory == ESettingsCategory::None)
    {
        RefreshUIFromCurrentSettings();
    }
}

void UGraphicsOptionsCategoryWidget::RefreshUIFromCurrentSettings()
{
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (!Settings) return;

    bIsRefreshing = true;

    // --- Resolution ---
    if (ResolutionCombo)
    {
        FIntPoint CurrentRes = Settings->GetScreenResolution();
        int32 ResIndex = ResolutionsArray.Find(CurrentRes);
        
        if (ResIndex != INDEX_NONE) 
        {
            ResolutionCombo->SetSelectedIndex(ResIndex);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GRAPHICS_WIDGET: Rezolūcija nav atrasta sarakstā!"));
        }
    }

    // --- Quality ---
    if (QualityCombo) 
    {
        int32 Qual = Settings->GetOverallScalabilityLevel();
        QualityCombo->SetSelectedIndex(Qual);
    }

    // --- Window Mode ---
    if (WindowModeCombo)
    {
        int32 Mode = (int32)Settings->GetFullscreenMode();
        WindowModeCombo->SetSelectedIndex(Mode);
    }

    bIsRefreshing = false;
}

void UGraphicsOptionsCategoryWidget::HandleResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (bIsRefreshing) return;
    if (SelectionType == ESelectInfo::Direct) return;

    int32 Index = ResolutionCombo->GetSelectedIndex();
    if (ResolutionsArray.IsValidIndex(Index))
    {
        if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
        {
            Settings->SetScreenResolution(ResolutionsArray[Index]);
            // Nemainām ApplySettings šeit, tikai marķējam kā Pending
            if (UUIOptionsMenuBase* Parent = GetParentOptions()) 
            {
                Parent->MarkCategoryPending(ESettingsCategory::Graphics);
            }
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
