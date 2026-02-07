#include "UI/Base/OptionsCategory/Audio/UAudioOptionsCategoryWidget.h"

#include "Components/PanelWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h" // Nepieciešams castam

#include "UI/Base/UIOptionsMenuBase.h" // Svarīgi: Īstais headeris

void UAudioOptionsCategoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (MasterSlider) MasterSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleMasterChanged);
    if (MusicSlider)  MusicSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleMusicChanged);
    if (SFXSlider)    SFXSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleSFXChanged);

    // Pirmo reizi ielādējam datus
    RefreshFromParent();
}

void UAudioOptionsCategoryWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    // Label iestatīšana paliek kā tev bija
}

UUIOptionsMenuBase* UAudioOptionsCategoryWidget::GetParentOptions() const
{
    // Izmantojam efektīvāku metodi, lai atrastu "tēva" logrīku hierarhijā
    return Cast<UUIOptionsMenuBase>(GetTypedOuter<UUIOptionsMenuBase>());
}

void UAudioOptionsCategoryWidget::HandleMasterChanged(float Value)
{
    if (bIsRefreshing) return;
    if (UUIOptionsMenuBase* Parent = GetParentOptions())
    {
        Parent->SetAudioOption(EAudioOption::Master, Value);
    }
}

void UAudioOptionsCategoryWidget::HandleMusicChanged(float Value)
{
    if (bIsRefreshing) return;
    if (UUIOptionsMenuBase* Parent = GetParentOptions())
    {
        Parent->SetAudioOption(EAudioOption::Music, Value);
    }
}

void UAudioOptionsCategoryWidget::HandleSFXChanged(float Value)
{
    if (bIsRefreshing) return;
    if (UUIOptionsMenuBase* Parent = GetParentOptions())
    {
        Parent->SetAudioOption(EAudioOption::SFX, Value);
    }
}

void UAudioOptionsCategoryWidget::HandleSettingsChanged(ESettingsCategory ChangedCategory)
{
    // Ja spēlētājs nospiež "Cancel", Parent izsauks OnSettingsChanged(None)
    if (ChangedCategory == ESettingsCategory::Audio || ChangedCategory == ESettingsCategory::None)
    {
        RefreshFromParent();
    }
}

void UAudioOptionsCategoryWidget::RefreshFromParent()
{
    UUIOptionsMenuBase* Parent = GetParentOptions();
    if (!Parent) return;

    bIsRefreshing = true;

    if (MasterSlider) MasterSlider->SetValue(Parent->GetPendingMasterVolume());
    if (MusicSlider)  MusicSlider->SetValue(Parent->GetPendingMusicVolume());
    if (SFXSlider)    SFXSlider->SetValue(Parent->GetPendingSFXVolume());

    bIsRefreshing = false;
}
