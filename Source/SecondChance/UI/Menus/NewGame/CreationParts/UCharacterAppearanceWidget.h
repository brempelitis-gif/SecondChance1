#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Structs/FCharacterCustomizationData.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h" // Tavs Button include
#include "UI/Base/MenuSlider/MenuSliderWidget.h" // Tavs Slider include
#include "UI/Base/MenuEditableText/UMenuEditableTextWidget.h"
#include "UI/Base/MenuCheckBox/MenuCheckBoxWidget.h"
#include "UCharacterAppearanceWidget.generated.h"

class UMenuCheckBoxWidget;
class UMenuEditableTextWidget;
class ACharacterSetupActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextStepRequested);
UCLASS()
class SECONDCHANCE_API UCharacterAppearanceWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// --- UI Elementi (Tavas klases) ---
	UPROPERTY(meta = (BindWidget))
	UMenuEditableTextWidget* NameInput;

	UPROPERTY(meta = (BindWidget))
	UMenuCheckBoxWidget* GenderCheckBox; // CheckBox vienkāršībai True = Female, False = Male

	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* HeightSlider;

	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* WeightSlider;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* RotateLeftBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* RotateRightBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* BackBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* NextBtn;

	// --- Labels priekš PreConstruct ---
	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText GenderLabelText = FText::FromString("Gender :");

	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText HeightLabelText = FText::FromString("Height :");

	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText NameLabelText = FText::FromString("Name :");

	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText WeightLabelText = FText::FromString("Weight :");

	UPROPERTY(EditAnywhere, Category = "UI|Buttons")
	FText BackBtnLabelText = FText::FromString("Back");

	UPROPERTY(EditAnywhere, Category = "UI|Buttons")
	FText NextBtnLabelText = FText::FromString("Next");

	UPROPERTY(EditAnywhere, Category = "UI|Buttons")
	FText RotateLeftBtnLabelText = FText::FromString("<");

	UPROPERTY(EditAnywhere, Category = "UI|Buttons")
	FText RotateRightBtnLabelText = FText::FromString(">");

	// --- Parametri ---
	UPROPERTY(EditAnywhere, Category = "Appearance|Rotation")
	float RotationSpeed = 120.0f; // Grādi sekundē
	
	// --- Dati ---
	FCharacterCustomizationData CurrentData;
    
	UPROPERTY()
	ACharacterSetupActor* PreviewActor;

	// --- Rotācijas stāvoklis ---
	bool bIsRotatingLeft = false;
	bool bIsRotatingRight = false;

	// --- Handleri ---
	UFUNCTION() void HandleNameChanged(const FText& Text);
	UFUNCTION() void HandleGenderChanged(bool bIsChecked);
	UFUNCTION() void HandleHeightChanged(float Value);
	UFUNCTION() void HandleWeightChanged(float Value);
	UFUNCTION() void HandleBackClicked();
	UFUNCTION() void HandleNextClicked();

	// Jauni handleri pogu turēšanai
	UFUNCTION() void StartRotateLeft() { bIsRotatingLeft = true; }
	UFUNCTION() void StopRotateLeft() { bIsRotatingLeft = false; }
	UFUNCTION() void StartRotateRight() { bIsRotatingRight = true; }
	UFUNCTION() void StopRotateRight() { bIsRotatingRight = false; }

private:
	void FindPreviewActor();// Palīgfunkcija
	void UpdateNextButtonState(); // Validācija
	void BindButtons();
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnNextStepRequested OnNextStepRequested;
};