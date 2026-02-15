#include "Splash.h"
#include "Components/TextBlock.h"

void USplash::NativeConstruct()
{
	Super::NativeConstruct();

	if (LoadingText)
	{
		LoadingText->SetText(FText::FromString("Loading..."));
	}

	// Šeit varētu palaid animāciju, ja tāda ir definēta Blueprintā
	// PlayAnimation(FadeInAnimation);
}

void USplash::SetLoadingText(FText NewText)
{
	if (LoadingText)
	{
		LoadingText->SetText(NewText);
	}
}
