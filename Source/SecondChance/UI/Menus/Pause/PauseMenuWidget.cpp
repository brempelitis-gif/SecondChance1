#include "UI/Menus/Pause/PauseMenuWidget.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "MyGameInstance.h"
#include "Core/Subsystems/UIConfig.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Piesaistām klikšķus
	if (ResumeBtn) ResumeBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleResume);
	if (SaveBtn)   SaveBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleSave);
	if (LoadBtn)   LoadBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleLoad);
	if (OptionsBtn) OptionsBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleOptions);
	if (QuitBtn)    QuitBtn->OnClicked.AddDynamic(this, &UPauseMenuWidget::HandleQuit);
}

void UPauseMenuWidget::HandleResume()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI) GI->TogglePauseMenu(); // Izmantojam to pašu Toggle loģiku
}

void UPauseMenuWidget::HandleSave()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		// Izmantojam tavu jau esošo funkciju!
		// Tā kā mēs šobrīd nezinām, vai šis ir jauns seivs vai esoša pārrakstīšana, 
		// mēs varam izmantot to pašu datu struktūru.
		FString NewSlot = GI->CreateNewSaveGame(GI->FinalCharacterData);
        
		// Parādām apstiprinājumu caur tavu UIManager
		if (GI->GetUIManager())
		{
			GI->GetUIManager()->PushConfirmationPopup(FText::FromString("Game Saved!"), 2.0f);
		}
	}
}

void UPauseMenuWidget::HandleLoad()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && GI->UIConfig->LoadGameMenuClass)
	{
		// Atveram Load Menu virs pauzes izvēlnes
		UUserWidget* LoadMenu = CreateWidget<UUserWidget>(this, GI->UIConfig->LoadGameMenuClass);
		GI->GetUIManager()->PushWidget(LoadMenu);
	}
}

void UPauseMenuWidget::HandleOptions()
{
}

void UPauseMenuWidget::HandleQuit()
{
	UGameplayStatics::OpenLevel(this, FName("L_MainMenu"));
}
