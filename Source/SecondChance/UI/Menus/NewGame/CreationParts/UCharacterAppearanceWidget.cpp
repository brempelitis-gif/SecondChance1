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
    // Ja tavām pogām ir SetText funkcija:
    if (BackBtn) BackBtn->SetLabel(FText::FromString("Back"));
    if (NextBtn) NextBtn->SetLabel(FText::FromString("Next"));

    if (RotateLeftBtn) RotateLeftBtn->SetLabel(FText::FromString("<"));
    if (RotateRightBtn) RotateRightBtn->SetLabel(FText::FromString(">"));
}

void UCharacterAppearanceWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    FindPreviewActor();

    // 1. Ievade
    if (NameInput) NameInput->OnTextChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleNameChanged);
    
    // 2. Dropdown (Gender)
    if (GenderCheckBox) GenderCheckBox->OnCheckStateChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleGenderChanged);
    
    // 3. Slider (Height) - Izmantojam tavu OnValueChanged delegātu
    if (HeightSlider) HeightSlider->OnValueChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleHeightChanged);

    // 4. Slider (Weight) - Izmantojam tavu OnValueChanged delegātu
    if (WeightSlider) WeightSlider->OnValueChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleWeightChanged);
    
    // 5. Pogas - Izmanto savu pogu delegātu (piem. OnButtonClicked)
    // SVARĪGI: Izmantojam OnPressed un OnReleased gludai rotācijai
    // Ja tavai UMenuButtonWidget klasei nav šādu delegātu, tie ir jāpievieno tajā (bāzējoties uz UButton)
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
    //if (RotateLeftBtn) RotateLeftBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleRotateLeft);
    //if (RotateRightBtn) RotateRightBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleRotateRight);
    
    if (BackBtn) BackBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleBackClicked);
    if (NextBtn) NextBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleNextClicked);

    // Sākotnējā pogas stāvokļa pārbaude
    if (NextBtn) NextBtn->SetIsEnabled(false);
}

void UCharacterAppearanceWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (CachedPreviewActor)
    {
        if (bIsRotatingLeft)
        {
            CachedPreviewActor->AddActorLocalRotation(FRotator(0, -RotationSpeed * InDeltaTime, 0));
        }
        if (bIsRotatingRight)
        {
            CachedPreviewActor->AddActorLocalRotation(FRotator(0, RotationSpeed * InDeltaTime, 0));
        }
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
        // Ja atrasts, saglabājam kešā un ielādējam sākotnējos datus
        CachedPreviewActor = PreviewActor;
        CachedPreviewActor->UpdatePreview(CurrentData);
    }
}

// --- HANDLERI ---

void UCharacterAppearanceWidget::HandleNameChanged(const FText& Text)
{
    CurrentData.PlayerName = Text.ToString();
    UpdateNextButtonState();
}
void UCharacterAppearanceWidget::HandleGenderChanged(bool bIsChecked)
{
    // Tavā struktūrā ir bIsMale. Ja Checkbox ir ieķeksēts, pieņemsim, ka tā ir sieviete.
    CurrentData.bIsMale = !bIsChecked; 
    
    if (CachedPreviewActor)
    {
        CachedPreviewActor->UpdatePreview(CurrentData);
    }
}

void UCharacterAppearanceWidget::HandleHeightChanged(float Value)
{
    CurrentData.HeightScale = Value; // Saglabājam datus struktūrā
    
    if (CachedPreviewActor)
    {
        // Sūtam visu struktūru, lai aktieris pārrēķina Z mērogu
        CachedPreviewActor->UpdatePreview(CurrentData);
    }
}

void UCharacterAppearanceWidget::HandleWeightChanged(float Value)
{
    CurrentData.WeightScale = Value; // Saglabājam datus struktūrā
    
    if (CachedPreviewActor)
    {
        // Sūtam visu struktūru, lai aktieris pārrēķina X/Y mērogu
        CachedPreviewActor->UpdatePreview(CurrentData);
    }
}

void UCharacterAppearanceWidget::HandleBackClicked()
{
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
        GI->AsyncLoadGameLevel(FName("L_MainMenu"));
    }
    //UGameplayStatics::OpenLevel(this, FName("/Game/ManaSpele/Levels/L_MainMenu"));
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