#pragma once

#include "CoreMinimal.h"
#include "UI/Menus/Options/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "UGraphicsOptionsCategoryWidget.generated.h"

class UMenuDropdownWidget;
class UMenuSliderWidget;
class UMenuCheckBoxWidget;
class UUIOptionsMenuBase; // Izmantojam tavu jauno klasi

UCLASS()
class SECONDCHANCE_API UGraphicsOptionsCategoryWidget : public UOptionsCategoryBaseWidget
{
    GENERATED_BODY()

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

    // --- BindWidgets ---
    UPROPERTY(meta = (BindWidget))
    UMenuDropdownWidget* ResolutionCombo;

    UPROPERTY(meta = (BindWidget))
    UMenuDropdownWidget* WindowModeCombo;

    UPROPERTY(meta = (BindWidget))
    UMenuDropdownWidget* QualityCombo;

    UPROPERTY(meta = (BindWidget))
    UMenuCheckBoxWidget* VSyncCheckBox;

    UPROPERTY(meta = (BindWidget))
    UMenuSliderWidget* ResolutionScaleSlider;

    // Labels
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText ResolutionComboLabel = FText::FromString("Resolution");
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText WindowModeComboLabel = FText::FromString("Window Mode");
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText QualityComboLabel = FText::FromString("Quality");
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText VSyncCheckBoxLabel = FText::FromString("VSync");
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
    FText ResolutionScaleLabel = FText::FromString("Resolution Scale");

    // --- Handleri ---
    UFUNCTION() void HandleResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION() void HandleQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION() void HandleWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
    UFUNCTION() void HandleVSyncChanged(bool bIsChecked);
    UFUNCTION() void HandleResolutionScaleChanged(float Value);

    /** Override no bāzes klases */
    virtual void HandleSettingsChanged(ESettingsCategory ChangedCategory) override;

    void RefreshUIFromCurrentSettings();

private:
    void PopulateComboBoxes();
    UUIOptionsMenuBase* GetParentOptions() const;
    
    TArray<FIntPoint> ResolutionsArray;
    const TArray<FString> QualityLabels = { TEXT("Low"), TEXT("Medium"), TEXT("High"), TEXT("Epic"), TEXT("Cinematic") };
    const TArray<FString> WindowModeLabels = { TEXT("Fullscreen"), TEXT("Windowed Fullscreen"), TEXT("Windowed") };

    bool bIsRefreshing = false;
};
