#pragma once

#include "Blueprint/UserWidget.h"
#include "USplashScreenWidget.generated.h"

UCLASS()
class SECONDCHANCE_API USplashScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LoadingBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PercentText;

public:
	void UpdateProgress(float Alpha);
};
