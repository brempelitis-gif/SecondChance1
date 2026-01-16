#include "MyGameInstance.h"
#include "Core/Subsystems/UIManagerSubsystem.h"

void UMyGameInstance::Init()
{
	Super::Init();

	if (UUIManagerSubsystem* UIManager = GetSubsystem<UUIManagerSubsystem>())
	{
		// Piesaistām Config
		UIManager->UIConfig = UIConfig;

		// Automātiski startē splash
		UIManager->ShowSplashScreen();
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