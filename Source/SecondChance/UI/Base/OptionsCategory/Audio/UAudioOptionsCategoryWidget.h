#pragma once

#include "UI/Base/OptionsCategory/OptionsCategoryBaseWidget.h"
#include "UI/Settings/AudioOptionType.h"
#include "UAudioOptionsCategoryWidget.generated.h"

class UMenuSliderWidget;
class UOptionsBaseWidget;

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

	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleMasterChanged(float Value);

	UFUNCTION()
	void HandleMusicChanged(float Value);

	UFUNCTION()
	void HandleSFXChanged(float Value);

	/** Pārmantots no Base: reaģē uz iestatījumu maiņu (Apply/Cancel) */
	virtual void HandleSettingsChanged(ESettingsCategory ChangedCategory) override;

	void RefreshFromParent();

private:
	/** Palīgs, lai droši atrastu OptionsBaseWidget */
	UOptionsBaseWidget* GetParentOptions() const;

	/** Karogs, lai Refresh laikā slaideri neizsauktu Handle funkcijas */
	bool bIsRefreshing = false;
};
