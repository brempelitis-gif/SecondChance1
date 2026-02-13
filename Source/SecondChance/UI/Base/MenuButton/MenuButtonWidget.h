#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"

class UButton;
class UTextBlock;

// Definējam delegātus nospiešanai un atlaišanai
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonReleased);

/**
 * Base class for all menu buttons (Apply, Cancel, Play, etc.)
 */
UCLASS(Abstract, BlueprintType)
class SECONDCHANCE_API UMenuButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* ================================
	 * EVENTS
	 * ================================ */

	UPROPERTY(BlueprintAssignable, Category="MenuButton")
	FOnMenuButtonClicked OnClicked;

	// Jaunie notikumi gludai rotācijai
	UPROPERTY(BlueprintAssignable, Category="MenuButton")
	FOnMenuButtonPressed OnPressed;

	UPROPERTY(BlueprintAssignable, Category="MenuButton")
	FOnMenuButtonReleased OnReleased;

	/* ================================
	 * API
	 * ================================ */

	UFUNCTION(BlueprintCallable, Category="MenuButton")
	void SetEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="MenuButton")
	void SetLabel(const FText& InText);

protected:
	// Skaņas, ko varēsi iestatīt Blueprintā
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* HoverSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* ClickSound;

	// Pārrakstām Native peles notikumus
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnInitialized() override;

	/* ================================
	 * BINDINGS
	 * ================================ */

	UPROPERTY(meta = (BindWidget))
	UButton* Button;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LabelText;

private:
	// Iekšējie handleri, kas "pārsūta" signālus
	UFUNCTION() void HandleButtonClicked();
	UFUNCTION() void HandleButtonPressed();
	UFUNCTION() void HandleButtonReleased();
};

