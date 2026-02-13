#include "UCharacterSkillTreeWidget.h"

#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


void UCharacterSkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Šis izpildīsies gan Editorā, gan spēles sākumā
	if (BackBtn)
	{
		BackBtn->SetLabel(BackButtonText);
	}

	if (PlayBtn)
	{
		PlayBtn->SetLabel(PlayButtonText);
	}
}

void UCharacterSkillTreeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (BackBtn)
	{
		BackBtn->OnClicked.AddDynamic(this, &UCharacterSkillTreeWidget::HandleBackClicked);
	}

	if (PlayBtn)
	{
		PlayBtn->OnClicked.AddDynamic(this, &UCharacterSkillTreeWidget::HandlePlayClicked);
	}
}

void UCharacterSkillTreeWidget::UpdateUI()
{
}

void UCharacterSkillTreeWidget::HandleBackClicked()
{
	// Izsaucam delegātu, ko klausās UCharacterCreationMain
	OnBackStepRequested.Broadcast();
}
void UCharacterSkillTreeWidget::HandlePlayClicked()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		// 1. NEVIS pārrakstām, bet PIEVIENOJAM skilus klāt jau esošajiem datiem
		// Pieņemot, ka tev CurrentData iekš SkillTree satur skilus:
		//GI->FinalCharacterData.Strength = this->CurrentData.Strength;
		//GI->FinalCharacterData.Agility = this->CurrentData.Agility;
		// ... pievieno visus pārējos skilus ...

		// 2. SAGLABĀJAM DISKĀ (Tikai vienu reizi!)
		// Šī funkcija atgriež ID, ko izmantosim screenshotam
		FString UniqueSlotID = GI->CreateNewSaveGame(GI->FinalCharacterData);

		// 3. Uzņemam portreta screenshot ar unikālo ID
		FString FileName = UniqueSlotID + "_Portrait";
		FScreenshotRequest::RequestScreenshot(FileName, false, false);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Game Saved & Portrait captured."));

		// 4. Asinhronā ielāde
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [GI]()
		{
		   if (GI)
		   {
			 GI->AsyncLoadGameLevel(FName("L_GameLevel"));
		   }
		}, 0.2f, false);
	}
}