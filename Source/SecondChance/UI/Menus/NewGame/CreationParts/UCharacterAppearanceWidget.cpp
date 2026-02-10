#include "UCharacterAppearanceWidget.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/MenuDropdown/MenuDropdownWidget.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"

void UCharacterAppearanceWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    
    // Uzstādām tekstus izmantojot tavas klases metodes
    if (NameInput) NameInput->SetLabel(NameLabelText);
    if (GenderCombo) GenderCombo->SetLabel(GenderLabelText);
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
    // Atrod pirmo CharacterSetupActor līmenī
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

void UCharacterAppearanceWidget::HandleNameChanged(const FText& Text)
{
    CurrentData.PlayerName = Text.ToString();
    // Atļaujam Next tikai tad, ja vārds nav tukšs
    if (NextBtn) NextBtn->SetIsEnabled(!Text.IsEmpty());
}

void UCharacterAppearanceWidget::HandleGenderChanged(bool bIsChecked)
{
    bIsFemaleSelected = bIsChecked;
    
    if (PreviewActor)
    {
        PreviewActor->UpdateGenderVisuals(bIsFemaleSelected);
        
        // Pārrēķinām augstumu, jo sievietēm skala ir citādāka
        float CurrentSliderVal = HeightSlider ? HeightSlider->GetValue() : 0.5f;
        PreviewActor->UpdateHeight(CurrentSliderVal, bIsFemaleSelected);
    }
}

void UCharacterAppearanceWidget::HandleHeightChanged(float Value)
{
    if (PreviewActor)
    {
        PreviewActor->UpdateHeight(Value, bIsFemaleSelected);
    }
}

void UCharacterAppearanceWidget::HandleWeightChanged(float Value)
{
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
    // Šeit vēlāk notiks pāreja uz Skill Tree
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Switching to Skill Tree..."));
    // Tā vietā, lai šeit darītu visu loģiku, mēs vienkārši "paziņojam" Masteram
    OnNextStepRequested.Broadcast();
}
