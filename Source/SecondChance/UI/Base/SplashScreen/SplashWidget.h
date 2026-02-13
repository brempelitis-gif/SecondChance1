#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SplashWidget.generated.h"

/**
 * C++ bāzes klase Splash Screen logrīkam
 */
UCLASS()
class SECONDCHANCE_API USplashWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// BindWidget automātiski sasaistīs mainīgo ar Blueprint elementu ar tādu pašu nosaukumu
	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundImage;

	UPROPERTY(meta = (BindWidget))
	class UCircularThrobber* LoadingCircle;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LoadingText;

	// Izpildās, kad logrīks tiek izveidots
	virtual void NativeConstruct() override;

public:
	// Funkcija, lai dinamiski mainītu tekstu (piem. "Loading Level...", "Optimizing Shaders...")
	void SetLoadingText(FText NewText);
};