#include "UI/Menus/Pause/PauseMenuWidget.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Components/BackgroundBlur.h"
#include "MyGameInstance.h"
#include "Core/Save/SCSaveGame.h"
#include "Core/Subsystems/UIConfig.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"



void UPauseMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// 1. Automātiski uzstādām tekstus pogām jau iekš Editora
	if (ResumeBtn)  ResumeBtn->SetLabel(ResumeBtnLabel);
	if (SaveBtn)    SaveBtn->SetLabel(SaveBtnLabel);
	if (LoadBtn)    LoadBtn->SetLabel(LoadBtnLabel);
	if (OptionsBtn) OptionsBtn->SetLabel(OptionsBtnLabel);
	if (QuitBtn)    QuitBtn->SetLabel(QuitBtnLabel);
    
	// 2. Regulējam Blur caur kodu (vai atstāj to darīt Editorā)
	if (BG_Blur) 
	{
		BG_Blur->SetBlurStrength(5.0f);
	}
}

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindButtons();
}

void UPauseMenuWidget::BindButtons()
{
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
    if (!GI) return;

    if (!GI->CurrentSaveSlotName.IsEmpty())
    {
        // 1. Pārrakstām esošo failu
        USCSaveGame* SaveObj = Cast<USCSaveGame>(UGameplayStatics::CreateSaveGameObject(USCSaveGame::StaticClass()));
        SaveObj->PlayerData = GI->FinalCharacterData;
        UGameplayStatics::SaveGameToSlot(SaveObj, GI->CurrentSaveSlotName, 0);
        
        // 2. Atjaunojam datumu Master Indexā
        GI->UpdateMasterIndexDate(GI->CurrentSaveSlotName);
    }
    else
    {
        // Jauna spēle (bez ielādes) -> Izmantojam tavu jauno seivu loģiku
        GI->CreateNewSaveGame(GI->FinalCharacterData);
    }

    if (GI->GetUIManager()) GI->GetUIManager()->PushConfirmationPopup(FText::FromString("Game Saved!"), 2.0f);
}
/*void UPauseMenuWidget::HandleSave()
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
*/
void UPauseMenuWidget::HandleLoad()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && GI->UIConfig->LoadGameMenuClass)
	{
		// Atveram Load Menu virs pauzes izvēlnes
		UUserWidget* LoadMenu = CreateWidget<UUserWidget>(this, GI->UIConfig->LoadGameMenuClass);
		GI->GetUIManager()->PushWidget(LoadMenu, false, true); // Pievienojam pa virsu
	}
}

void UPauseMenuWidget::HandleOptions()
{
}

void UPauseMenuWidget::HandleQuit()
{
	// DROŠĪBAI: Pirms ejam uz Main Menu, OBLIGĀTI noņemam pauzi un atjaunojam laiku!
	// Citādi Main Menu ielādēsies ar iestrēgušu TimeDilation = 0
	UGameplayStatics::SetGamePaused(GetWorld(), false);
    
	UGameplayStatics::OpenLevel(this, FName("L_MainMenu"));
}
