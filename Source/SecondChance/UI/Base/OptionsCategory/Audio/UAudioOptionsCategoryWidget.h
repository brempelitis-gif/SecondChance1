#pragma once

#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "UI/Settings/AudioOptionType.h"
#include "UAudioOptionsCategoryWidget.generated.h"

class USlider;
class UMenuSliderWidget;
class UTextBlock;
class UUIManagerSubsystem;

/**
 * Audio options category widget.
 * - Bind three sliders (Master/Music/SFX) in the WBP.
 * - Uses UIManagerSubsystem to preview and mark pending changes.
 */
UCLASS()
class SECONDCHANCE_API UAudioOptionsCategoryWidget : public UOptionsCategoryBaseWidget
{
	GENERATED_BODY()

protected:
	// Widgets - name these exactly in the UMG blueprint or change BindWidgetOptional to BindWidge
	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* MasterSlider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText MasterSliderLabel = FText::FromString("Master");


	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* MusicSlider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText MusicSliderLabel = FText::FromString("Music");

	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* SFXSlider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Options|Labels")
	FText SFXSliderLabel = FText::FromString("SFX");

	// Called after the parent - ensure UIManager from base is available
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	// Slider handlers
	UFUNCTION()
	void HandleMasterChanged(float Value);

	UFUNCTION()
	void HandleMusicChanged(float Value);

	UFUNCTION()
	void HandleSFXChanged(float Value);

	// Listen to UIManager settings changes to refresh UI when apply/cancel happen
	UFUNCTION()
	void HandleManagerSettingsChanged(ESettingsCategory ChangedCategory);

	// Helper to (re)read pending values from UIManager and set sliders
	void RefreshFromManager();
};
