
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

    auto* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->SplashWidgetClass)
    {
        // Izveidojam logrīku
        SplashWidget = CreateWidget<USplashScreenWidget>(GetWorld(), UIMan->UIConfig->SplashWidgetClass);
        
        if (SplashWidget)
        {
            // Reģistrējam caur sistēmu
            UIMan->PushWidget(SplashWidget);
        }
    }

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
    // Pārliecināmies, ka logrīks vēl eksistē un ir redzams
    if (IsValid(SplashWidget) && SplashWidget->IsInViewport())
    {
       float Progress = GetAsyncLoadPercentage(NextLevelName);
       SplashWidget->UpdateProgress(Progress / 100.0f);
    }
}
