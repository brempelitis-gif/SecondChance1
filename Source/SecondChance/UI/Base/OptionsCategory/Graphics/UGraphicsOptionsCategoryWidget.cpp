#include "UI/Base/OptionsCategory/Graphics/UGraphicsOptionsCategoryWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h"

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
    RefreshUIFromCurrentSettings();
}

UOptionsBaseWidget* UGraphicsOptionsCategoryWidget::GetParentOptions() const
{
    UWidget* Current = GetParent();
    while (Current)
    {
        if (UOptionsBaseWidget* Options = Cast<UOptionsBaseWidget>(Current)) return Options;
        Current = Current->GetParent();
    }
    return nullptr;
}

void UGraphicsOptionsCategoryWidget::PopulateComboBoxes()
{
    if (!ResolutionCombo) return;

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

    bIsRefreshing = true;

    if (ResolutionCombo)
    {
       int32 ResIndex = ResolutionsArray.Find(Settings->GetScreenResolution());
       if (ResIndex != INDEX_NONE) ResolutionCombo->SetSelectedIndex(ResIndex);
    }

    if (QualityCombo) QualityCombo->SetSelectedIndex(Settings->GetOverallScalabilityLevel());
    if (WindowModeCombo) WindowModeCombo->SetSelectedIndex((int32)Settings->GetFullscreenMode());
    if (VSyncCheckBox) VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());
    if (ResolutionScaleSlider) ResolutionScaleSlider->SetValue(Settings->GetResolutionScaleNormalized());

    bIsRefreshing = false;
}

void UGraphicsOptionsCategoryWidget::HandleResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct || bIsRefreshing) return;
    
    int32 Index = ResolutionCombo->GetSelectedIndex();
    if (ResolutionsArray.IsValidIndex(Index))
    {
       if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
       {
          Settings->SetScreenResolution(ResolutionsArray[Index]);
          if (UOptionsBaseWidget* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
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
          if (UOptionsBaseWidget* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
       }
    }
}

void UGraphicsOptionsCategoryWidget::HandleWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (bIsRefreshing || SelectionType == ESelectInfo::Direct) return;
    
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
       Settings->SetFullscreenMode((EWindowMode::Type)WindowModeCombo->GetSelectedIndex());
       if (UOptionsBaseWidget* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
    }
}

void UGraphicsOptionsCategoryWidget::HandleVSyncChanged(bool bIsChecked)
{
    if (bIsRefreshing) return;
    
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
       Settings->SetVSyncEnabled(bIsChecked);
       if (UOptionsBaseWidget* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
    }
}

void UGraphicsOptionsCategoryWidget::HandleResolutionScaleChanged(float Value)
{
    if (bIsRefreshing) return;
    
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
       Settings->SetResolutionScaleNormalized(Value);
       if (UOptionsBaseWidget* Parent = GetParentOptions()) Parent->MarkCategoryPending(ESettingsCategory::Graphics);
    }
}
