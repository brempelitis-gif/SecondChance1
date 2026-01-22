#pragma once

#include "Blueprint/UserWidget.h"
#include "UIBaseWidget.generated.h"

UCLASS(Abstract, BlueprintType)
class SECONDCHANCE_API UUIBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	// --- Public API ---
	UFUNCTION(BlueprintCallable, Category="UI")
	void FadeIn();

	UFUNCTION(BlueprintCallable, Category="UI")
	void FadeOutAndRemove();

protected:
	// --- Hooks priekš bērnu widgetiem ---
	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnWidgetShown();

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnWidgetHidden();

	// Fade parametri (rediģējami BP)
	UPROPERTY(EditDefaultsOnly, Category="UI|Fade")
	float FadeDuration = 0.5f;

private:
	void PlayFade(float From, float To, bool bRemoveAfter);

private:
	FTimerHandle FadeTimer;
};