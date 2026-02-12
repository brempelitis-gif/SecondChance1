#include "UCharacterAppearanceWidget.h"

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
    if (RotateLeftBtn) RotateLeftBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleRotateLeft);
    if (RotateRightBtn) RotateRightBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleRotateRight);
    
    if (BackBtn) BackBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleBackClicked);
    if (NextBtn) NextBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleNextClicked);

    // Sākotnējā pogas stāvokļa pārbaude
    if (NextBtn) NextBtn->SetIsEnabled(false);
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

void UCharacterAppearanceWidget::HandleRotateLeft()
{
    if (CachedPreviewActor) CachedPreviewActor->AddActorLocalRotation(FRotator(0, -15, 0));
}

void UCharacterAppearanceWidget::HandleRotateRight()
{
    if (CachedPreviewActor) CachedPreviewActor->AddActorLocalRotation(FRotator(0, 15, 0));
}

void UCharacterAppearanceWidget::HandleBackClicked()
{
    UGameplayStatics::OpenLevel(this, FName("/Game/ManaSpele/Levels/L_MainMenu"));
}
void UCharacterAppearanceWidget::UpdateNextButtonState()
{
    bool bNameValid = NameInput && !NameInput->GetText().IsEmpty();
    // Pievieno citas pārbaudes (Age, etc.)
    
    if (NextBtn) NextBtn->SetIsEnabled(bNameValid);
}
void UCharacterAppearanceWidget::HandleNextClicked()
{
    
    if (OnNextStepRequested.IsBound())
    {
        OnNextStepRequested.Broadcast();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("APPEARANCE: No one is listening to OnNextStepRequested!"));
    }
}