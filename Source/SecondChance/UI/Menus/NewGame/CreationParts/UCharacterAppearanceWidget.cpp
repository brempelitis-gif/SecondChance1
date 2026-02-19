#include "UCharacterAppearanceWidget.h"

#include "MyGameInstance.h"
#include "CharacterActor/ACharacterSetupActor.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"

void UCharacterAppearanceWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    // Uzstādām tekstus izmantojot tavas klases metodes
    if (NameInput) NameInput->SetLabel(NameLabelText);
    if (GenderCheckBox) GenderCheckBox->SetLabel(GenderLabelText);
    if (HeightSlider) HeightSlider->SetLabel(HeightLabelText);
    if (WeightSlider) WeightSlider->SetLabel(WeightLabelText);
    
    if (BackBtn) BackBtn->SetLabel(BackBtnLabelText);
    if (NextBtn) NextBtn->SetLabel(NextBtnLabelText);

    if (RotateLeftBtn) RotateLeftBtn->SetLabel(RotateLeftBtnLabelText);
    if (RotateRightBtn) RotateRightBtn->SetLabel(RotateRightBtnLabelText);
}

void UCharacterAppearanceWidget::NativeConstruct()
{
    Super::NativeConstruct();
    FindPreviewActor();

    FTimerHandle CameraInitTimer;
    GetWorld()->GetTimerManager().SetTimer(CameraInitTimer, [this]()
    {
        if (PreviewActor)
        {
            APlayerController* PC = GetOwningPlayer();
            APawn* Pwn = PC ? PC->GetPawn() : nullptr;

            if (PC && Pwn)
            {
                // 1. Iestatām skata mērķi uz PAWN (nevis uz tēlu!)
                PC->SetViewTarget(Pwn);
                // 1. POZĪCIJA: 
                // Y=250 (attālums), Z=90 (augstums)
                // X = -50.0f (pabīdām kameru pa kreisi, lai tēls vizuāli būtu labajā pusē starp bultiņām)
                FVector CamPos = PreviewActor->GetActorLocation() + FVector(-150.0f, 250.0f, 90.0f);
                
                // 2. MĒRĶIS: 
                // Mēs skatāmies nevis tieši uz tēlu, bet uz punktu, kas ir "nobīdīts" 
                // Šis palīdzēs tēlam izskatīties dabiski, nevis sašķiebtam
                FVector LookAtTarget = PreviewActor->GetActorLocation() + FVector(-10.0f, 0.0f, 90.0f);
                
                FRotator CamRot = (LookAtTarget - CamPos).Rotation();
                // 2. Ja tiešām vajag SetMobility, tad caur RootComponent:
                if (Pwn->GetRootComponent())
                {
                    Pwn->GetRootComponent()->SetMobility(EComponentMobility::Movable);
                }

                // 3. Teleportējam un piespiežam skatīties caur šo Pawn
                PC->GetPawn()->SetActorLocationAndRotation(CamPos, CamRot);
                PC->SetControlRotation(CamRot);
                PC->SetViewTargetWithBlend(Pwn, 0.0f); // Šis piespiež redzēt no Pawn acīm
            }
        }
    }, 0.2f, false); // 0.2s ir drošākais laiks
    // ------------------------
    // 1. Iestatām Sākuma Vērtības UI (Slaideri pa vidu)
    float DefaultSliderValue = 0.5f;

    if (HeightSlider) HeightSlider->SetValue(DefaultSliderValue);
    if (WeightSlider) WeightSlider->SetValue(DefaultSliderValue);
    
    // Pieņemsim: Checkbox OFF = Vīrietis, Checkbox ON = Sieviete
    if (GenderCheckBox) GenderCheckBox->SetIsChecked(false);

    // 2. Piespiedu kārtā izsaucam "Changed" funkcijas, 
    // lai tās aprēķinātu cm/kg un nosūtītu datus uz 3D aktieri.
    HandleHeightChanged(DefaultSliderValue);
    HandleWeightChanged(DefaultSliderValue);
    HandleGenderChanged(false);
}

void UCharacterAppearanceWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    

    BindButtons();
}


void UCharacterAppearanceWidget::HandleHeightChanged(float Value)
{
    // DEFINĪCIJA: 0.0 = 160cm (Scale 0.9), 1.0 = 200cm (Scale 1.1)
    // Vidus (0.5) = 180cm (Scale 1.0)
    
    float MinScale = 0.9f;
    float MaxScale = 1.1f;
    
    // Lineārā interpolācija (Lerp)
    CurrentData.HeightScale = FMath::Lerp(MinScale, MaxScale, Value);

    // Vizuālais teksts (cm)
    if (HeightSlider)
    {
        // Pieņemsim, ka bāzes modelis (Scale 1.0) ir 180cm
        float DisplayHeight = CurrentData.HeightScale * 180.0f;
       // FString Label = FString::Printf(TEXT("%.0f cm"), DisplayHeight);
       // HeightSlider->SetValueUI(FText::FromString(Label));
        HeightSlider->SetValueUI(DisplayHeight);
    }

    if (PreviewActor) PreviewActor->UpdatePreview(CurrentData);
}

void UCharacterAppearanceWidget::HandleWeightChanged(float Value)
{
    // 1. Aprēķinām mērogu priekš 3D tēla (lai resnums mainītos vizuāli)
    // 0.0 = 0.85 (tievs), 1.0 = 1.15 (resns)
    float MinScale = 0.85f;
    float MaxScale = 1.15f;
    CurrentData.WeightScale = FMath::Lerp(MinScale, MaxScale, Value);

    // 2. Aprēķinām Svaru priekš UI (lai rādītu kg)
    // 0.0 = 50kg, 1.0 = 110kg
    float MinKg = 50.0f;
    float MaxKg = 110.0f;
    float DisplayWeight = FMath::Lerp(MinKg, MaxKg, Value);

    // 3. Nosūtam skaitli uz tavu slaideri
    if (WeightSlider)
    {
        WeightSlider->SetValueUI(DisplayWeight);
    }

    if (PreviewActor) PreviewActor->UpdatePreview(CurrentData);
}

void UCharacterAppearanceWidget::HandleGenderChanged(bool bIsChecked)
{
    // Checkbox TRUE = Sieviete, FALSE = Vīrietis
    CurrentData.bIsMale = !bIsChecked;

    if (PreviewActor) PreviewActor->UpdatePreview(CurrentData);
}

void UCharacterAppearanceWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    if (PreviewActor)
    {
        if (bIsRotatingLeft) PreviewActor->AddActorLocalRotation(FRotator(0, -RotationSpeed * InDeltaTime, 0));
        if (bIsRotatingRight) PreviewActor->AddActorLocalRotation(FRotator(0, RotationSpeed * InDeltaTime, 0));
    }
}

void UCharacterAppearanceWidget::FindPreviewActor()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterSetupActor::StaticClass());
    PreviewActor = Cast<ACharacterSetupActor>(FoundActor);

    if (!PreviewActor)
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterAppearanceWidget: Neizdevās atrast CharacterSetupActor līmenī!"));
    }
    else
    {
        PreviewActor->UpdatePreview(CurrentData);
    }
}

// --- HANDLERI ---

void UCharacterAppearanceWidget::HandleNameChanged(const FText& Text)
{
    CurrentData.PlayerName = Text.ToString();
    UpdateNextButtonState();
}

void UCharacterAppearanceWidget::HandleBackClicked()
{
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
        GI->AsyncLoadGameLevel(FName("L_MainMenu"));
    }
}
void UCharacterAppearanceWidget::UpdateNextButtonState()
{
    bool bNameValid = NameInput && !NameInput->GetText().IsEmpty();
    // Pievieno citas pārbaudes (Age, etc.)
    
    if (NextBtn) NextBtn->SetIsEnabled(bNameValid);
}
void UCharacterAppearanceWidget::HandleNextClicked()
{
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
        // 1. Sinhronizējam vārdu no InputBox
        if (NameInput) CurrentData.PlayerName = NameInput->GetText().ToString();

        // 2. Ierakstām GameInstance 'pagaidu' atmiņā
        GI->FinalCharacterData = this->CurrentData;
    }

    if (OnNextStepRequested.IsBound())
    {
        OnNextStepRequested.Broadcast();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("APPEARANCE: No one is listening to OnNextStepRequested!"));
    }
}
void UCharacterAppearanceWidget::BindButtons()
{
    // 1. Ievade
    if (NameInput) NameInput->OnTextChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleNameChanged);
    
    // 2. Dropdown (Gender)
    if (GenderCheckBox) GenderCheckBox->OnCheckStateChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleGenderChanged);
    
    // 3. Slider (Height) - Izmantojam tavu OnValueChanged delegātu
    if (HeightSlider) HeightSlider->OnValueChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleHeightChanged);

    // 4. Slider (Weight) - Izmantojam tavu OnValueChanged delegātu
    if (WeightSlider) WeightSlider->OnValueChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleWeightChanged);
    
    // 5. Pogas - Izmanto savu pogu delegātu (piem. OnClicked)
    // SVARĪGI: Izmantojam OnPressed un OnReleased gludai rotācijai
    if (RotateLeftBtn)
    {
        RotateLeftBtn->OnPressed.AddDynamic(this, &UCharacterAppearanceWidget::StartRotateLeft);
        RotateLeftBtn->OnReleased.AddDynamic(this, &UCharacterAppearanceWidget::StopRotateLeft);
    }

    if (RotateRightBtn)
    {
        RotateRightBtn->OnPressed.AddDynamic(this, &UCharacterAppearanceWidget::StartRotateRight);
        RotateRightBtn->OnReleased.AddDynamic(this, &UCharacterAppearanceWidget::StopRotateRight);
    }
    
    if (BackBtn) BackBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleBackClicked);
    if (NextBtn) NextBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleNextClicked);

    // Sākotnējā pogas stāvokļa pārbaude
    if (NextBtn) NextBtn->SetIsEnabled(false);
}