#pragma once
#include "Components/EditableTextBox.h"
#include "UI/Base/UBaseLabeledWidget.h"
#include "UI/Base/UIBaseWidget.h"
#include "MenuSliderWidget.generated.h"

class UTextBox;
class USlider;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnMenuSliderValueChanged,
	float,
	Value
);

UCLASS()
class SECONDCHANCE_API UMenuSliderWidget : public UBaseLabeledWidget
{
	GENERATED_BODY()

public:
	// --- KONFIGURĀCIJA ---

	// Ja TRUE (noklusējums): Reizina vērtību ar 100 (piem. 0.5 -> 50). Der Audio/Settings.
	// Ja FALSE: Rāda vērtību kā ir (piem. 180 -> 180). Der Character Creator.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider Settings")
	bool bIsPercentage = true;

	// Ja TRUE (noklusējums): Slaideris pats nomaina tekstu uzreiz, kad to bīda (0-1 robežās).
	// Ja FALSE: Slaideris tekstu nemaina. To dara tavs ārējais kods (CharacterAppearanceWidget).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider Settings")
	bool bAutoUpdateText = true;
	
	UFUNCTION(BlueprintCallable)
	void SetValue(float InValue);
	void SetValueUI(float InValue);

	UFUNCTION(BlueprintCallable)
	float GetValue() const;

	UPROPERTY(BlueprintAssignable)
	FOnMenuSliderValueChanged OnValueChanged;
	
	//UFUNCTION(BlueprintCallable, Category="Label")
	//void SetLabel(const FText& InText);
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void HandleSliderChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	USlider* Slider;

//	UPROPERTY(meta = (BindWidget))
//	UTextBlock* LabelText;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Value;
};

