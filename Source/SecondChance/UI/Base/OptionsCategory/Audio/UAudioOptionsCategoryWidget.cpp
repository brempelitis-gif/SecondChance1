#include "UI/Base/OptionsCategory/Audio/UAudioOptionsCategoryWidget.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Base/MenuSlider/MenuSliderWidget.h"

void UAudioOptionsCategoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// UIManager is set in UOptionsCategoryBaseWidget::NativeOnInitialized, but be safe:
	if (!UIManager)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			UIManager = GI->GetSubsystem<UUIManagerSubsystem>();
		}
	}

	// Bind sliders
	if (MasterSlider)
	{
		MasterSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleMasterChanged);
	}
	if (MusicSlider)
	{
		MusicSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleMusicChanged);
	}
	if (SFXSlider)
	{
		SFXSlider->OnValueChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleSFXChanged);
	}

	// Listen for manager changes so we refresh when Apply/Cancel occurs
	if (UIManager)
	{
		UIManager->OnSettingsChanged.AddDynamic(this, &UAudioOptionsCategoryWidget::HandleManagerSettingsChanged);
	}

	// Initialize slider values from pending (so if the user already changed values they are shown)
	RefreshFromManager();
}
void UAudioOptionsCategoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	MasterSlider->SetLabel(MasterSliderLabel);
	MusicSlider->SetLabel(MusicSliderLabel);
	SFXSlider->SetLabel(SFXSliderLabel);
}

void UAudioOptionsCategoryWidget::NativeDestruct()
{
	// Clean up delegates
	if (MasterSlider)
	{
		MasterSlider->OnValueChanged.RemoveDynamic(this, &UAudioOptionsCategoryWidget::HandleMasterChanged);
	}
	if (MusicSlider)
	{
		MusicSlider->OnValueChanged.RemoveDynamic(this, &UAudioOptionsCategoryWidget::HandleMusicChanged);
	}
	if (SFXSlider)
	{
		SFXSlider->OnValueChanged.RemoveDynamic(this, &UAudioOptionsCategoryWidget::HandleSFXChanged);
	}

	if (UIManager)
	{
		UIManager->OnSettingsChanged.RemoveDynamic(this, &UAudioOptionsCategoryWidget::HandleManagerSettingsChanged);
	}

	Super::NativeDestruct();
}

void UAudioOptionsCategoryWidget::HandleMasterChanged(float Value)
{
	if (!UIManager) return;
	UIManager->SetAudioOption(EAudioOption::Master, Value);
}

void UAudioOptionsCategoryWidget::HandleMusicChanged(float Value)
{
	if (!UIManager) return;
	UIManager->SetAudioOption(EAudioOption::Music, Value);
}

void UAudioOptionsCategoryWidget::HandleSFXChanged(float Value)
{
	if (!UIManager) return;
	UIManager->SetAudioOption(EAudioOption::SFX, Value);
}

void UAudioOptionsCategoryWidget::HandleManagerSettingsChanged(ESettingsCategory ChangedCategory)
{
	// Refresh when audio category or global (None) changes
	if (ChangedCategory == Category || ChangedCategory == ESettingsCategory::None)
	{
		RefreshFromManager();
	}
}

void UAudioOptionsCategoryWidget::RefreshFromManager()
{
	if (!UIManager) return;

	// Use pending values so the UI reflects current pending state (or after cancel/apply)
	if (MasterSlider)
	{
		MasterSlider->SetValue(UIManager->GetPendingMasterVolume());
	}
	if (MusicSlider)
	{
		MusicSlider->SetValue(UIManager->GetPendingMusicVolume());
	}
	if (SFXSlider)
	{
		SFXSlider->SetValue(UIManager->GetPendingSFXVolume());
	}
}
