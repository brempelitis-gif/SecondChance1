#include "SplashWidget.h"
#include "Components/Image.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"

void USplashWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LoadingText)
	{
		LoadingText->SetText(FText::FromString("Loading..."));
	}

	// Šeit varētu palaid animāciju, ja tāda ir definēta Blueprintā
	// PlayAnimation(FadeInAnimation);
}

void USplashWidget::SetLoadingText(FText NewText)
{
	if (LoadingText)
	{
		LoadingText->SetText(NewText);
	}
}
