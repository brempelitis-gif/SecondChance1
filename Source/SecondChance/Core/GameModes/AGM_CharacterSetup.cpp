#include "AGM_CharacterSetup.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Core/Subsystems/UIConfig.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Menus/NewGame/UCharacterCreationMain.h"

AGM_CharacterSetup::AGM_CharacterSetup()
{
}

void AGM_CharacterSetup::BeginPlay()
{
	Super::BeginPlay();

	UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    
	if (UIMan && UIMan->UIConfig)
	{
		if (UIMan->UIConfig->NewGameWidgetClass)
		{
			UCharacterCreationMain* CharacterWidget = CreateWidget<UCharacterCreationMain>(GetWorld(), UIMan->UIConfig->NewGameWidgetClass);
          
			if (CharacterWidget)
			{
				UIMan->PushWidget(CharacterWidget);
			}
			else
			{
				// Šis izvadīsies, ja NewGameWidgetClass nav UCharacterCreationMain tips
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GAMEMODE: Failed to cast Widget to UCharacterCreationMain!"));
             
				// Alternatīva: parādām jebko, kas tur ir ielikts, lai redzētu, kas notiek
				UUserWidget* SimpleWidget = CreateWidget<UUserWidget>(GetWorld(), UIMan->UIConfig->NewGameWidgetClass);
				if (SimpleWidget) {
					SimpleWidget->AddToViewport();
				}
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GAMEMODE: NewGameWidgetClass is EMPTY in UIConfig!"));
		}
	}
}
