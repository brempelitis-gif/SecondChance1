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
    if (GenderCombo) GenderCombo->SetLabel(GenderLabelText);
    if (HeightSlider) HeightSlider->SetLabel(HeightLabelText);
    
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
    if (GenderCombo) GenderCombo->OnSelectionChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleGenderChanged);
    
    // 3. Slider (Height) - Izmantojam tavu OnValueChanged delegātu
    if (HeightSlider) HeightSlider->OnValueChanged.AddDynamic(this, &UCharacterAppearanceWidget::HandleHeightChanged);
    
    // 4. Pogas - Izmanto savu pogu delegātu (piem. OnButtonClicked)
    if (RotateLeftBtn) RotateLeftBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleRotateLeft);
    if (RotateRightBtn) RotateRightBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleRotateRight);
    
    if (BackBtn) BackBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleBackClicked);
    if (NextBtn) NextBtn->OnClicked.AddDynamic(this, &UCharacterAppearanceWidget::HandleNextClicked);

    // Sākotnējā pogas stāvokļa pārbaude
    if (NextBtn) NextBtn->SetIsEnabled(false);
}

void UCharacterAppearanceWidget::FindPreviewActor()
{
    AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterPreviewActor::StaticClass());
    CachedPreviewActor = Cast<ACharacterPreviewActor>(FoundActor);
    
    if (CachedPreviewActor) CachedPreviewActor->UpdatePreview(CurrentData);
}

void UCharacterAppearanceWidget::HandleNameChanged(const FText& Text)
{
    CurrentData.PlayerName = Text.ToString();
    // Atļaujam Next tikai tad, ja vārds nav tukšs
    if (NextBtn) NextBtn->SetIsEnabled(!Text.IsEmpty());
}

void UCharacterAppearanceWidget::HandleGenderChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct) return;

    CurrentData.bIsMale = (GenderCombo->GetSelectedIndex() == 0);
    if (CachedPreviewActor) CachedPreviewActor->UpdatePreview(CurrentData);
}

void UCharacterAppearanceWidget::HandleHeightChanged(float Value)
{
    CurrentData.HeightScale = Value;
    if (CachedPreviewActor) CachedPreviewActor->UpdatePreview(CurrentData);
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
    //UGameplayStatics::OpenLevel(this, FName("L_MainMenu"));
}

void UCharacterAppearanceWidget::HandleNextClicked()
{
    // Šeit vēlāk notiks pāreja uz Skill Tree
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Switching to Skill Tree..."));
}
