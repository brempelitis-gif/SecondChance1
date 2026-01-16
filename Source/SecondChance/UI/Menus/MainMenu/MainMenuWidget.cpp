#include "MainMenuWidget.h"
#include "Core/Save/SaveGameSubsystem.h"
#include "Core/Save/SaveGameData.h"
#include "UI/Base/SaveSlotWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);

	RefreshSaveList();
}

void UMainMenuWidget::RefreshSaveList()
{
	SaveListBox->ClearChildren();

	if (USaveGameSubsystem* SaveSubsystem =
		GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
	{
		TArray<USaveGameData*> Saves = SaveSubsystem->GetAllSaves();

		// A7: kārtošana pēc datuma (ja vajag – var mainīt)
		Saves.Sort([](const USaveGameData& A, const USaveGameData& B)
		{
			return A.SaveDate > B.SaveDate;
		});
		
		for (USaveGameData* Save : Saves)
		{
			USaveSlotWidget* SaveSlotWidget =
				CreateWidget<USaveSlotWidget>(this, SaveSlotWidgetClass);

			SaveSlotWidget->Init(Save);
			SaveListBox->AddChild(SaveSlotWidget);
		}
	}
}

void UMainMenuWidget::OnNewGameClicked()
{
	if (USaveGameSubsystem* SaveSubsystem =
		GetGameInstance()->GetSubsystem<USaveGameSubsystem>())
	{
		SaveSubsystem->CreateNewSave(TEXT("Player"));
		RefreshSaveList();
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}
