#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuButtonWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuButtonClicked);

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
	UFUNCTION()
	void HandleButtonClicked();
};

