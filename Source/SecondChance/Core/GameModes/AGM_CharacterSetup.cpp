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

	// Iegūstam UI sistēmu
	UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    
	if (UIMan && UIMan->UIConfig && UIMan->UIConfig->NewGameWidgetClass)
	{
		// Izveidojam Character Setup logrīku
		UCharacterCreationMain* CharacterWidget = CreateWidget<UCharacterCreationMain>(GetWorld(), UIMan->UIConfig->NewGameWidgetClass);
		if (CharacterWidget)
		{
			// Tā kā šis ir jauns līmenis, Stack ir tukšs. PushWidget to parādīs kā galveno.
			UIMan->PushWidget(CharacterWidget);
            
			// Nodrošinām, ka pele ir redzama un fokuss ir uz UI
			APlayerController* PC = GetWorld()->GetFirstPlayerController();
			if (PC)
			{
				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(CharacterWidget->TakeWidget());
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
		}
	}
}
