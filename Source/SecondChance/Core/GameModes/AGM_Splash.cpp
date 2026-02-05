
#include "AGM_Splash.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Levels/USplashScreenWidget.h"

AGM_Splash::AGM_Splash()
{
}

void AGM_Splash::BeginPlay()
{
	Super::BeginPlay();

	if (SplashWidgetClass)
	{
		SplashWidget = CreateWidget<USplashScreenWidget>(GetWorld(), SplashWidgetClass);
		if (SplashWidget) SplashWidget->AddToViewport();
	}

	// Pēc 1 sekundes sāk ielādi
	GetWorldTimerManager().SetTimerForNextTick(this, &AGM_Splash::StartAsyncLoad);
}

void AGM_Splash::StartAsyncLoad()
{
	// Pārliecinies, ka NextLevelName nav tukšs
	if (NextLevelName.IsNone()) {
		UE_LOG(LogTemp, Error, TEXT("AGM_Splash: NextLevelName ir TUKŠS!"));
		return;
	}

	LoadPackageAsync(NextLevelName.ToString(),
	   FLoadPackageAsyncDelegate::CreateLambda([this](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
	   {
		  if (Result == EAsyncLoadingResult::Succeeded)
		  {
			 UGameplayStatics::OpenLevel(this, NextLevelName);
		  }
	   }),
	   0, PKG_ContainsMap);
}

void AGM_Splash::UpdateLoadingProgress()
{
	if (SplashWidget)
	{
		float Progress = GetAsyncLoadPercentage(NextLevelName);
        
		// Nododam datus Widgetam (Progress ir 0-100 robežās)
		SplashWidget->UpdateProgress(Progress / 100.0f);
	}
}