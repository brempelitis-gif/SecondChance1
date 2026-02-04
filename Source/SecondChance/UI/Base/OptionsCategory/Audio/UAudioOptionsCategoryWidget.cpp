#include "UI/Base/OptionsCategory/Audio/UAudioOptionsCategoryWidget.h"

#include "Components/PanelWidget.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"
#include "UI/Menus/Options/OptionsBaseWidget.h" // Nepieciešams castam

void UAudioOptionsCategoryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // Bind slider events
    if (MasterSlider) MasterSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleMasterChanged);
    if (MusicSlider)  MusicSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleMusicChanged);
    if (SFXSlider)    SFXSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleSFXChanged);

    RefreshFromParent();
}

void UAudioOptionsCategoryWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (MasterSlider) MasterSlider->SetLabel(MasterSliderLabel);
    if (MusicSlider)  MusicSlider->SetLabel(MusicSliderLabel);
    if (SFXSlider)    SFXSlider->SetLabel(SFXSliderLabel);
}

UOptionsBaseWidget* UAudioOptionsCategoryWidget::GetParentOptions() const
{
    UWidget* Current = GetParent();
    while (Current)
    {
        if (UOptionsBaseWidget* Options = Cast<UOptionsBaseWidget>(Current))
        {
            return Options;
        }
        Current = Current->GetParent();
    }
    return nullptr;
}

void UAudioOptionsCategoryWidget::HandleMasterChanged(float Value)
{
    if (bIsRefreshing) return;
    if (UOptionsBaseWidget* Parent = GetParentOptions())
    {
        Parent->SetAudioOption(EAudioOption::Master, Value);
    }
}

void UAudioOptionsCategoryWidget::HandleMusicChanged(float Value)
{
    if (bIsRefreshing) return;
    if (UOptionsBaseWidget* Parent = GetParentOptions())
    {
        Parent->SetAudioOption(EAudioOption::Music, Value);
    }
}

void UAudioOptionsCategoryWidget::HandleSFXChanged(float Value)
{
    if (bIsRefreshing) return;
    if (UOptionsBaseWidget* Parent = GetParentOptions())
    {
        Parent->SetAudioOption(EAudioOption::SFX, Value);
    }
}

void UAudioOptionsCategoryWidget::HandleSettingsChanged(ESettingsCategory ChangedCategory)
{
    // Ja mainījās Audio vai tika izsaukts globāls reset (None)
    if (ChangedCategory == ESettingsCategory::Audio || ChangedCategory == ESettingsCategory::None)
    {
        RefreshFromParent();
    }
}

void UAudioOptionsCategoryWidget::RefreshFromParent()
{
    UOptionsBaseWidget* Parent = GetParentOptions();
    if (!Parent) return;

    bIsRefreshing = true; // Paceļam karogu

    if (MasterSlider) MasterSlider->SetValue(Parent->GetPendingMasterVolume());
    if (MusicSlider)  MusicSlider->SetValue(Parent->GetPendingMusicVolume());
    if (SFXSlider)    SFXSlider->SetValue(Parent->GetPendingSFXVolume());

    bIsRefreshing = false; // Nolaižam karogu
}
