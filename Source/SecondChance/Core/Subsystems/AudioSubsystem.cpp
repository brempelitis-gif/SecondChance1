#include "Core/Subsystems/AudioManagerSubsystem.h"
#include "Core/SecondChanceConstants.h"        // Mūsu konstantes
#include "Core/Save/AudioSettingsSaveGame.h"   // Tavs SaveGame fails
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "UI/Settings/AudioOptionType.h"

void UAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Mēģinām atrast SoundClasses, ja tās nav iestatītas (Optional, ja izmanto BP config)
    // Bet labāk tās iestatīt Editorā, izveidojot Blueprint klasi no šī Subsystema,
    // vai arī ielādēt šeit ar ConstructorHelpers (nav ieteicams Subsystemiem).
    
    // 1. Ielādējam datus no diska
    LoadSettings();
}

void UAudioManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UAudioManagerSubsystem::SetPendingVolume(EAudioOption Option, float Value)
{
    // 1. Atjaunojam pagaidu struktūru
    switch (Option)
    {
        case EAudioOption::Master: PendingSettings.MasterVolume = Value; break;
        case EAudioOption::Music:  PendingSettings.MusicVolume  = Value; break;
        case EAudioOption::SFX:    PendingSettings.SFXVolume    = Value; break;
    }

    // 2. Uzreiz pielietojam skaņai ("Live Preview"), bet nesaglabājam failā
    ApplyVolumeToSoundClasses(PendingSettings);
}

void UAudioManagerSubsystem::ApplySettings()
{
    // Apstiprinām izmaiņas
    SavedSettings = PendingSettings;
    SaveSettings();
}

void UAudioManagerSubsystem::CancelSettings()
{
    // Atgriežamies pie pēdējā saglabātā stāvokļa
    PendingSettings = SavedSettings;
    ApplyVolumeToSoundClasses(SavedSettings);
}

void UAudioManagerSubsystem::LoadSettings()
{
    bool bLoaded = false;

    if (UGameplayStatics::DoesSaveGameExist(SC_Constants::AudioSaveSlot, SC_Constants::AudioUserIndex))
    {
        UAudioSettingsSaveGame* Save = Cast<UAudioSettingsSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SC_Constants::AudioSaveSlot, SC_Constants::AudioUserIndex)
        );

        if (Save)
        {
            SavedSettings.MasterVolume = Save->MasterVolume;
            SavedSettings.MusicVolume  = Save->MusicVolume;
            SavedSettings.SFXVolume    = Save->SFXVolume;
            bLoaded = true;
        }
    }

    // Ja nav saglabāts, izmantojam noklusējuma (1.0f)
    if (!bLoaded)
    {
        SavedSettings = FAudioSettingsData(); // Default constructor (1.0f)
    }

    // Sinhronizējam Pending ar Saved un pielietojam
    PendingSettings = SavedSettings;
    ApplyVolumeToSoundClasses(SavedSettings);
}

void UAudioManagerSubsystem::SaveSettings()
{
    UAudioSettingsSaveGame* Save = Cast<UAudioSettingsSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass())
    );

    if (Save)
    {
        Save->MasterVolume = SavedSettings.MasterVolume;
        Save->MusicVolume  = SavedSettings.MusicVolume;
        Save->SFXVolume    = SavedSettings.SFXVolume;

        UGameplayStatics::SaveGameToSlot(Save, SC_Constants::AudioSaveSlot, SC_Constants::AudioUserIndex);
    }
}

void UAudioManagerSubsystem::ApplyVolumeToSoundClasses(const FAudioSettingsData& Settings)
{
    InternalSetSoundClassVolume(MasterSoundClass, Settings.MasterVolume);
    InternalSetSoundClassVolume(MusicSoundClass,  Settings.MusicVolume);
    InternalSetSoundClassVolume(SFXSoundClass,    Settings.SFXVolume);
}

void UAudioManagerSubsystem::InternalSetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
    if (SoundClass)
    {
        SoundClass->Properties.Volume = Volume;
    }
}