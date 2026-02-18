#include "UCharacterSkillTreeWidget.h"

#include "MyGameInstance.h"
#include "GameFramework/Character.h"
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
	APlayerController* PC = GetOwningPlayer();

	if (GI && PC)
	{
		FString UniqueSlotID = GI->CreateNewSaveGame(GI->FinalCharacterData);

		// 1. UZŅEMAM PILNO BILDI
		FString FullBodyName = UniqueSlotID + "_Full";
		FScreenshotRequest::RequestScreenshot(FullBodyName, false, false);
        
		// 2. SAGATAVOJAM SEJAS FOKUSU
		ACharacter* PreviewChar = Cast<ACharacter>(PC->GetPawn());
		if (PreviewChar)
		{
			PrepareCameraForPortrait(PreviewChar);
		}

		GI->LastCapturedPortraitName = UniqueSlotID + "_Face";

		// 3. TAIMERIS SEJAS BILDEI (Dodam 0.2s, lai kamera paspēj "pārlēkt")
		FTimerHandle FaceCaptureTimer;
		GetWorld()->GetTimerManager().SetTimer(FaceCaptureTimer, [GI]()
		{
			FScreenshotRequest::RequestScreenshot(GI->LastCapturedPortraitName, false, false);
		}, 0.2f, false);

		// 4. TAIMERIS LĪMEŅA IELĀDEI (Dodam 1.2s, lai abas bildes paspēj nonākt diskā)
		// Šis ir drošāks laiks, īpaši uz lēnākiem diskiem.
		FTimerHandle LoadLevelTimer;
		GetWorld()->GetTimerManager().SetTimer(LoadLevelTimer, [GI]()
		{
			if (GI) GI->AsyncLoadGameLevel(FName("L_GameLevel"));
		}, 1.2f, false);
	}
}
void UCharacterSkillTreeWidget::PrepareCameraForPortrait(ACharacter* TargetChar)
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC || !TargetChar) return;

	// 1. Atrodam kameras pozīciju pie sejas
	// Pārliecinies, ka Mesh ir "head" sockets!
	FVector HeadLocation = TargetChar->GetMesh()->GetSocketLocation("head");
    
	// Novietojam kameru tieši pretī sejai (50 vienības uz priekšu)
	FVector CameraLocation = HeadLocation + (TargetChar->GetActorForwardVector() * 45.0f); 
	FRotator CameraRotation = (HeadLocation - CameraLocation).Rotation();

	// 2. Ja tev līmenī ir CameraActor, labāk kustināt to. 
	// Ja nē, tad PC->SetViewTarget(TargetChar) un tad grozām tēlu.
	// Bet vienkāršākais - pārvietojam PlayerController kameru:
	PC->SetControlRotation(CameraRotation);
	PC->SetAudioListenerOverride(TargetChar->GetMesh(), FVector::ZeroVector, FRotator::ZeroRotator);
    
	// Force set location (tikai uz mirkli)
	if(APawn* P = PC->GetPawn()) {
		P->SetActorLocation(CameraLocation);
	}
}