#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"
#include "Core/Subsystems/UIManagerSubsystem.h"

void UMyGameInstance::Init()
{
	Super::Init();

	if (UUIManagerSubsystem* UIManager = GetSubsystem<UUIManagerSubsystem>())
	{
		// Piesaistām Config
		UIManager->UIConfig = UIConfig;
	}
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
}

UUIManagerSubsystem* UMyGameInstance::GetUIManager() const
{
	return GetSubsystem<UUIManagerSubsystem>();
}

void UMyGameInstance::AsyncLoadGameLevel(FName LevelName)
{
	// 1. Parādām Splash Screen (šeit tu izsauc savu UI sistēmu)
	if (SplashScreenClass)
	{
		UUserWidget* Splash = CreateWidget<UUserWidget>(this, SplashScreenClass);
		if (Splash)
		{
			if (UUIManagerSubsystem* UIManager = GetSubsystem<UUIManagerSubsystem>())
			{
				UIManager->PushWidget(Splash);
			}
		}
	}
	// 2. Ceļa validācija un ielāde
	// Pārliecināmies, ka ceļš sākas ar /Game/
	FString LevelPath = LevelName.ToString();
	if (!LevelPath.StartsWith(TEXT("/Game/")))
	{
		// Pielāgo šo ceļu savai mapju struktūrai!
		LevelPath = TEXT("/Game/ManaSpele/Levels/") + LevelPath;
	}
	UE_LOG(LogTemp, Log, TEXT("AsyncLoading: Mēģinu ielādēt: %s"), *LevelPath);

	LoadPackageAsync(LevelPath,
		FLoadPackageAsyncDelegate::CreateLambda([this, LevelPath](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		{
			if (Result == EAsyncLoadingResult::Succeeded)
			{
				// Kad ielādēts, izmantojam nosaukumu bez ceļa priekš OpenLevel
				FName ShortLevelName = FName(*FPackageName::GetShortName(PackageName));
				UGameplayStatics::OpenLevel(this, ShortLevelName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AsyncLoading: Neizdevās ielādēt %s"), *LevelPath);
			}
		}),
		0, PKG_ContainsMap);
}

void UMyGameInstance::OnLevelLoaded()
{
}
