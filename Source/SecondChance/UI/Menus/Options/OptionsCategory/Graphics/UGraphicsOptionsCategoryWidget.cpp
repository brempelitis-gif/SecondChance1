#include "UI/Menus/Options/OptionsCategory/Graphics/UGraphicsOptionsCategoryWidget.h"
#include "GameFramework/GameUserSettings.h"
#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h"
#include "UI/Menus/Options/UIOptionsMenuBase.h"

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

    // Piesaistām handlerus
    if (ResolutionCombo) ResolutionCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionChanged);
    if (QualityCombo) QualityCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleQualityChanged);
    if (WindowModeCombo) WindowModeCombo->OnSelectionChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleWindowModeChanged);
    if (VSyncCheckBox) VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleVSyncChanged);
    if (ResolutionScaleSlider) ResolutionScaleSlider->OnValueChanged.AddDynamic(this, &UGraphicsOptionsCategoryWidget::HandleResolutionScaleChanged);
}

void UGraphicsOptionsCategoryWidget::NativeConstruct()
{
    Super::NativeConstruct(); // Šis izsauks RefreshWidgetUI no bāzes klases
    
    // 1. Vispirms sagatavojam sarakstus
    PopulateComboBoxes();
    
    // 2. Ielādējam datus (bez taimera, ja iespējams, bet 0.1s ir drošībai, ja ComboBox vēl nav uzģenerējis Slate objektus)
    RefreshUIFromCurrentSettings();
}

void UGraphicsOptionsCategoryWidget::RefreshUIFromCurrentSettings()
{
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    if (!Settings) return;

    // Uzstādām flagu, lai Handleri nedomātu, ka lietotājs maina iestatījumus
    bIsRefreshing = true;

    // --- 1. Resolution ---
    if (ResolutionCombo)
    {
        FIntPoint CurrentRes = Settings->GetScreenResolution();
        int32 ResIndex = ResolutionsArray.Find(CurrentRes);
        if (ResIndex != INDEX_NONE) ResolutionCombo->SetSelectedIndex(ResIndex);
    }

    // --- 2. Overall Quality ---
    if (QualityCombo) 
    {
        int32 Qual = Settings->GetOverallScalabilityLevel();
        // Ja Qual ir -1 (Custom), mēs parasti izvēlamies pēdējo vai atstājam kā ir
        if (Qual != -1)
        {
            QualityCombo->SetSelectedIndex(Qual);
        }
    }

    // --- 3. Window Mode ---
    if (WindowModeCombo)
    {
        int32 Mode = (int32)Settings->GetFullscreenMode();
        WindowModeCombo->SetSelectedIndex(Mode);
    }

    // --- 4. VSync (ŠIS BIJA IZTRŪKSTOŠAIS GABALS) ---
    if (VSyncCheckBox)
    {
        VSyncCheckBox->SetIsChecked(Settings->IsVSyncEnabled());
    }

    // --- 5. Resolution Scale (ŠIS ARĪ BIJA IZTRŪKSTOŠAIS) ---
    if (ResolutionScaleSlider)
    {
        ResolutionScaleSlider->SetValue(Settings->GetResolutionScaleNormalized());
    }

    bIsRefreshing = false;
}

// ================= HANDLERI =================

void UGraphicsOptionsCategoryWidget::RefreshWidgetUI()
{
    // Šeit mēs izsaucam to pašu loģiku, ko uzrakstījām iepriekš
    RefreshUIFromCurrentSettings();
    
    UE_LOG(LogTemp, Log, TEXT("Graphics UI atsvaidzināts caur C++ Override"));
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

// Šī funkcija vairs nav vajadzīga, jo viss ir iekļauts RefreshUIFromCurrentSettings
void UGraphicsOptionsCategoryWidget::SyncGraphicsUI() { }

void UGraphicsOptionsCategoryWidget::HandleSettingsChanged(ESettingsCategory ChangedCategory)
{
    if (ChangedCategory == ESettingsCategory::Graphics || ChangedCategory == ESettingsCategory::None)
    {
        RefreshUIFromCurrentSettings();
    }
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
UUIOptionsMenuBase* UGraphicsOptionsCategoryWidget::GetParentOptions() const
{
    return Cast<UUIOptionsMenuBase>(GetTypedOuter<UUIOptionsMenuBase>());
}
