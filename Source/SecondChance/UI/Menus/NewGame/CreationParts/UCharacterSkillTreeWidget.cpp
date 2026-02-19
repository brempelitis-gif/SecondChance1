#include "UCharacterSkillTreeWidget.h"
#include "UI/Menus/NewGame/CreationParts/CharacterActor/ACharacterSetupActor.h"
#include "MyGameInstance.h"
#include "CharacterActor/ACharacterSetupActor.h"
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

    if (!GI || !PC) return;

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterSetupActor::StaticClass());
    if (!FoundActor) return;

    FString UniqueSlotID = GI->CreateNewSaveGame(GI->FinalCharacterData);

	// 1. PIEBRAUCAM PIE SEJAS
	PrepareCameraForPortrait(FoundActor);

	FTimerHandle FaceCaptureTimer;
	GetWorld()->GetTimerManager().SetTimer(FaceCaptureTimer, [this, GI, UniqueSlotID, FoundActor, PC]()
	{
		FScreenshotRequest::RequestScreenshot(UniqueSlotID + "_Face", false, false);
		UE_LOG(LogTemp, Warning, TEXT("FOTO: Seja uzņemta."));

		// 2. TAIMERIS: FULL BODY
		FTimerHandle FullBodyTimer;
		GetWorld()->GetTimerManager().SetTimer(FullBodyTimer, [GI, UniqueSlotID, FoundActor, PC]()
		{
			// LABOJUMS: Izmantojam fiksētu nobīdi pa Y asi (180 vienības), 
			// nevis ForwardVector, lai bilde vienmēr būtu no priekšas.
			FVector FullCamPos = FoundActor->GetActorLocation() + FVector(0.0f, 180.0f, 80.0f);
			FRotator FullCamRot = (FoundActor->GetActorLocation() + FVector(0.0f, 0.0f, 80.0f) - FullCamPos).Rotation();
            
			if (APawn* SpecPawn = PC->GetPawn()) 
			{
				SpecPawn->SetActorLocationAndRotation(FullCamPos, FullCamRot);
			}
			PC->SetControlRotation(FullCamRot);

			// Dodam vienu kadru laiku kamerai nostāties
			FTimerHandle ShotTimer;
			GI->GetWorld()->GetTimerManager().SetTimer(ShotTimer, [UniqueSlotID]() {
				FScreenshotRequest::RequestScreenshot(UniqueSlotID + "_Full", false, false);
				UE_LOG(LogTemp, Warning, TEXT("FOTO: Pilnais augums uzņemts."));
			}, 0.1f, false);

			// 3. TAIMERIS: IELĀDĒJAM LĪMENI
			FTimerHandle LoadLevelTimer;
			GI->GetWorld()->GetTimerManager().SetTimer(LoadLevelTimer, [GI]()
			{
				if (GI) GI->AsyncLoadGameLevel(FName("L_GameLevel"));
			}, 0.6f, false);

		}, 0.5f, false);
	}, 0.5f, false);
}
void UCharacterSkillTreeWidget::PrepareCameraForPortrait(AActor* TargetActor)
{
	APlayerController* PC = GetOwningPlayer();
	if (!PC || !TargetActor) return;

	FVector HeadPos = TargetActor->GetActorLocation() + FVector(0, 0, 165.0f); // Aptuvenais galvas augstums
	FVector CamPos = HeadPos + FVector(0.0f, 65.0f, 0.0f); 
	FRotator CamRot = (HeadPos - CamPos).Rotation();

	// 1. Teleportējam Pawn
	if (APawn* SpecPawn = PC->GetPawn())
	{
		SpecPawn->SetActorLocationAndRotation(CamPos, CamRot);
	}
    
	// 2. !!! SVARĪGI !!! Piespiežam skatu būt no Pawn
	PC->SetViewTarget(PC->GetPawn());
	PC->SetControlRotation(CamRot);
}