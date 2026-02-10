#pragma once

#include "CoreMinimal.h"
#include "UI/Menus/Options//OptionsCategory/OptionsCategoryBaseWidget.h"
#include "UI/Settings/AudioOptionType.h"
#include "UAudioOptionsCategoryWidget.generated.h"

class UMenuSliderWidget;
class UUIOptionsMenuBase; // Izmantojam tavu jauno klasi

UCLASS()
class SECONDCHANCE_API UAudioOptionsCategoryWidget : public UOptionsCategoryBaseWidget
{
	GENERATED_BODY()

protected:
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

	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual  void NativeConstruct() override;
    
	/** No OptionsCategoryBaseWidget */
	virtual void HandleSettingsChanged(ESettingsCategory ChangedCategory) override;

private:
	UFUNCTION() void HandleMasterChanged(float Value);
	UFUNCTION() void HandleMusicChanged(float Value);
	UFUNCTION() void HandleSFXChanged(float Value);

	void RefreshFromParent();

	/** Meklējam UUIOptionsMenuBase, nevis OptionsBaseWidget */
	UUIOptionsMenuBase* GetParentOptions() const;

	bool bIsRefreshing = false;
};

