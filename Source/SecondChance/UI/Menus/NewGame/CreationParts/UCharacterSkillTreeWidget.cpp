#include "UCharacterSkillTreeWidget.h"
#include "UI/Menus/NewGame/CreationParts/CharacterActor/ACharacterSetupActor.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


void UCharacterSkillTreeWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	
	// Šis izpildīsies gan Editorā, gan spēles sākumā
	if (BackBtn) BackBtn->SetLabel(BackButtonText);
	if (PlayBtn) PlayBtn->SetLabel(PlayButtonText);
}
void UCharacterSkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GI = Cast<UMyGameInstance>(GetGameInstance());
}
void UCharacterSkillTreeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (BackBtn) BackBtn->OnClicked.AddDynamic(this, &UCharacterSkillTreeWidget::HandleBackClicked);
	if (PlayBtn) PlayBtn->OnClicked.AddDynamic(this, &UCharacterSkillTreeWidget::HandlePlayClicked);
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
    APlayerController* PC = GetOwningPlayer();
    // Pārbaudām klases līmeņa GI
    if (!GI || !PC) return;

    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterSetupActor::StaticClass());
    if (!FoundActor) return;
    
    this->SetVisibility(ESlateVisibility::Hidden);
    
    FString UniqueSlotID = GI->CreateNewSaveGame(GI->FinalCharacterData);

    PrepareCameraForPortrait(FoundActor);

    FTimerHandle FaceCaptureTimer;
    // NODODAM: [this, UniqueSlotID, FoundActor, PC] - GI tiks paņemts no 'this'
    GetWorld()->GetTimerManager().SetTimer(FaceCaptureTimer, [this, UniqueSlotID, FoundActor, PC]()
    {
       FScreenshotRequest::RequestScreenshot(UniqueSlotID + "_Face", false, false);
       UE_LOG(LogTemp, Warning, TEXT("FOTO: Seja uzņemta."));

       FTimerHandle FullBodyTimer;
       GetWorld()->GetTimerManager().SetTimer(FullBodyTimer, [this, UniqueSlotID, FoundActor, PC]()
       {
          FVector FullCamPos = FoundActor->GetActorLocation() + FVector(0.0f, 220.0f, 100.0f);
          FRotator FullCamRot = (FoundActor->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f) - FullCamPos).Rotation();
          
          if (APawn* SpecPawn = PC->GetPawn()) 
          {
             SpecPawn->SetActorLocationAndRotation(FullCamPos, FullCamRot);
          }
          PC->SetControlRotation(FullCamRot);

          FTimerHandle ShotTimer;
          // Šeit capture sarakstā pietiek ar [UniqueSlotID]
          GetWorld()->GetTimerManager().SetTimer(ShotTimer, [UniqueSlotID]() {
             FScreenshotRequest::RequestScreenshot(UniqueSlotID + "_Full", false, false);
          }, 0.1f, false);

          FTimerHandle LoadLevelTimer;
          // Šeit izmantojam [this], lai tiktu pie GI->AsyncLoad...
          GetWorld()->GetTimerManager().SetTimer(LoadLevelTimer, [this]()
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

	// 1. Aprēķinām galvas pozīciju (Z=165 ir labi)
	FVector HeadPos = TargetActor->GetActorLocation() + FVector(0.0f, 0.0f, 165.0f);
    
	// 2. PARALĒLĀ NOBĪDE: 
	// Pabīdām kameru nedaudz sānis (X=15), lai seja nebūtu tieši centrā un izskatītos dabiski
	float PortraitOffset = 15.0f; 
	FVector CamPos = HeadPos + FVector(PortraitOffset, 70.0f, 0.0f); 
	FVector LookAtTarget = HeadPos + FVector(PortraitOffset, 0.0f, 0.0f);
    
	FRotator CamRot = (LookAtTarget - CamPos).Rotation();

	if (APawn* SpecPawn = PC->GetPawn())
	{
		SpecPawn->SetActorLocationAndRotation(CamPos, CamRot);
	}
    
	PC->SetViewTarget(PC->GetPawn());
	PC->SetControlRotation(CamRot);
}