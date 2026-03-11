
#include "AGM_Splash.h"
#include "Blueprint/UserWidget.h"
#include "Core/Subsystems/UIConfig.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/SplashScreen/Splash.h"

AGM_Splash::AGM_Splash()
{
}
void AGM_Splash::BeginPlay()
{
    Super::BeginPlay();

	UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	if (UIMan && UIMan->UIConfig && UIMan->UIConfig->SplashWidgetClass)
	{
		// Izveidojam logrīku
		USplash* SplasheWidget = CreateWidget<USplash>(GetWorld(), UIMan->UIConfig->SplashWidgetClass);
		if (SplasheWidget)
		{
			// Uzstumjam to uz ekrāna
			UIMan->PushWidget(SplasheWidget);
		}
	}
	// Dosim Splash ekrānam vismaz 2 sekundes goda laika pirms sākam ielādi
	FTimerHandle SplashDelayHandle;
	GetWorldTimerManager().SetTimer(SplashDelayHandle, this, &AGM_Splash::StartAsyncLoad, 2.0f, false);

	
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

