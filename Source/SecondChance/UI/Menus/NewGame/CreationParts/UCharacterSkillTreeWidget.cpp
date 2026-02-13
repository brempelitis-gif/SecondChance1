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
	// 1. Saglabājam datus GameInstance, lai tie nepazūd
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->FinalCharacterData = CurrentData; // CurrentData ir tava FCharacterCustomizationData
	}

	// 2. Uzņemam portreta screenshot priekš Save Game saraksta
	FString SaveName = CurrentData.PlayerName.IsEmpty() ? TEXT("Hero") : CurrentData.PlayerName;
	FString FileName = SaveName + "_Portrait"; // Paplašinājumu .png Unreal pieliks pats
    
	FScreenshotRequest::RequestScreenshot(FileName, false, false);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Portrait captured. Loading level..."));

	// 3. Izsaucam asinhrono ielādi caur GameInstance pēc maza brīža (lai paspēj uzņemt bildi)
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [GI]()
	{
		if (GI)
		{
			GI->AsyncLoadGameLevel(FName("L_GameLevel"));
		}
	}, 0.2f, false);
}