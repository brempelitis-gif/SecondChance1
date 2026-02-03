#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Menus/NewGame/ACharacterPreviewActor.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h" // Tavs Button include
#include "UI/Base/MenuSlider/MenuSliderWidget.h" // Tavs Slider include
#include "UI/Base/MenuEditableText/UMenuEditableTextWidget.h"
#include "UCharacterAppearanceWidget.generated.h"

class UMenuDropdownWidget;
class UMenuEditableTextWidget;

// Definējam delegātu faila augšā
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStepNavigationRequested);
UCLASS()
class SECONDCHANCE_API UCharacterAppearanceWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	// --- UI Elementi (Tavas klases) ---
	UPROPERTY(meta = (BindWidget))
	UMenuEditableTextWidget* NameInput;

	UPROPERTY(meta = (BindWidget))
	UMenuDropdownWidget* GenderCombo;

	UPROPERTY(meta = (BindWidget))
	UMenuSliderWidget* HeightSlider;

	UPROPERTY(meta = (BindWidget))
	class UMenuSliderWidget* WeightSlider;

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
	FText GenderLabelText = FText::FromString("Gender");

	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText HeightLabelText = FText::FromString("Height");

	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText NameLabelText = FText::FromString("Name");

	UPROPERTY(EditAnywhere, Category = "Appearance Labels")
	FText WeightLabelText = FText::FromString("Weight");
	// --- Dati ---
	FCharacterCustomizationData CurrentData;
    
	UPROPERTY()
	ACharacterPreviewActor* CachedPreviewActor;

	// --- Handleri ---
	UFUNCTION() void HandleNameChanged(const FText& Text);
	UFUNCTION() void HandleGenderChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION() void HandleHeightChanged(float Value);
	UFUNCTION() void HandleWeightChanged(float Value);
	UFUNCTION() void HandleRotateLeft();
	UFUNCTION() void HandleRotateRight();
	UFUNCTION() void HandleBackClicked();
	UFUNCTION() void HandleNextClicked();

private:
	void FindPreviewActor();
public:
	UPROPERTY(BlueprintAssignable)
	FOnStepNavigationRequested OnNextStepRequested;
};