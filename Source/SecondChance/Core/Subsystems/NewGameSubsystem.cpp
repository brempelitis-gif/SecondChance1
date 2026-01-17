#include "NewGameSubsystem.h"
#include "Core/Save/SaveGameSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UNewGameSubsystem::StartNewGame()
{
	UE_LOG(LogTemp, Log, TEXT("L_CharacterSetup"));
	// Ielādē character setup scēnu
	UGameplayStatics::OpenLevel(this, "L_CharacterSetup");
	
}

void UNewGameSubsystem::ConfirmCharacter(const FString& PlayerName)
{
	PendingPlayerName = PlayerName;

	USaveGameSubsystem* SaveSubsystem =
		GetGameInstance()->GetSubsystem<USaveGameSubsystem>();

	SaveSubsystem->CreateNewSave(PendingPlayerName);

	// Start game
	UGameplayStatics::OpenLevel(this, "L_Gameplay");
}
