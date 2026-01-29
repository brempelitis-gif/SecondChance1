#pragma once

#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "Components/Slider.h"
#include "UGraphicsOptionsCategoryWidget.generated.h"

class UMenuDropdownWidget;
class UUIManagerSubsystem;
class UMenuSliderWidget;
class UMenuCheckBoxWidget;

UCLASS()
class SECONDCHANCE_API UGraphicsOptionsCategoryWidget : public UOptionsCategoryBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// --- BindWidgets (Jāatbilst nosaukumiem Blueprintā) ---
	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* ResolutionCombo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText ResolutionComboLabel = FText::FromString("Resolution");

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* WindowModeCombo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText WindowModeComboLabel = FText::FromString("Window Mode");

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* QualityCombo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText QualityComboLabel = FText::FromString("Quality");


	UPROPERTY(meta = (BindWidget))
	UMenuCheckBoxWidget* VSyncCheckBox;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText VSyncCheckBoxLabel = FText::FromString("VSync");

	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* ResolutionScaleSlider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText ResolutionScaleLabel = FText::FromString("ResolutionScale");


	// --- Iekšējie dati ---
	TArray<FIntPoint> ResolutionsArray;
	const TArray<FString> QualityLabels = { TEXT("Low"), TEXT("Medium"), TEXT("High"), TEXT("Epic"), TEXT("Cinematic") };
	const TArray<FString> WindowModeLabels = { TEXT("Fullscreen"), TEXT("Windowed Fullscreen"), TEXT("Windowed") };

	// --- Handleri ---
	UFUNCTION() void HandleResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION() void HandleQualityChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION() void HandleWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION() void HandleVSyncChanged(bool bIsChecked);
	UFUNCTION() void HandleResolutionScaleChanged(float Value);

	UFUNCTION() void RefreshUIFromCurrentSettings(ESettingsCategory ChangedCategory);

private:
	void PopulateComboBoxes();
};


