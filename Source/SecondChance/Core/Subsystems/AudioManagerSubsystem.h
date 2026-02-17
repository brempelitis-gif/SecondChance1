#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioManagerSubsystem.generated.h"

class USoundClass;
class UAudioSettingsSaveGame;

// Struktūra, kas satur visus skaļuma datus vienuviet
USTRUCT(BlueprintType)
struct FAudioSettingsData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Audio")
    float MasterVolume = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Audio")
    float MusicVolume = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Audio")
    float SFXVolume = 1.0f;
};

/**
 * Subsystem that handles Sound Classes, Volume Control, and Save/Load logic.
 * Accessible globally via GetGameInstance()->GetSubsystem<UAudioManagerSubsystem>();
 */
UCLASS()
class SECONDCHANCE_API UAudioManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // --- Lifecycle ---
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // --- Public API for UI ---
    
    // Maina "Pending" (pagaidu) skaļumu. Lietotājs dzird izmaiņas, bet tās vēl nav saglabātas.
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void SetPendingVolume(EAudioOption Option, float Value);

    // Saglabā "Pending" izmaiņas diskā (SaveGame).
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void ApplySettings();

    // Atceļ "Pending" izmaiņas un atgriež skaļumu uz pēdējo saglabāto.
    UFUNCTION(BlueprintCallable, Category = "Audio Manager")
    void CancelSettings();

    // Atgriež pašreizējos (Pending) iestatījumus, lai UI zinātu, kur nobīdīt slaiderus.
    UFUNCTION(BlueprintPure, Category = "Audio Manager")
    FAudioSettingsData GetCurrentAudioSettings() const { return PendingSettings; }

    // --- Configuration (Set these in Blueprint derived class or Code) ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Config")
    TObjectPtr<USoundClass> MasterSoundClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Config")
    TObjectPtr<USoundClass> MusicSoundClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Config")
    TObjectPtr<USoundClass> SFXSoundClass;

protected:
    // --- Internal State ---
    
    // Dati, kas ir saglabāti failā (HDD)
    UPROPERTY()
    FAudioSettingsData SavedSettings;

    // Dati, kas pašlaik ir spēkā (UI stāvoklis)
    UPROPERTY()
    FAudioSettingsData PendingSettings;

private:
    // --- Internal Logic ---
    void LoadSettings();
    void SaveSettings();
    
    // Fiziski nomaina SoundClass Volume
    void ApplyVolumeToSoundClasses(const FAudioSettingsData& Settings);
    
    // Palīgfunkcija vienai klasei
    void InternalSetSoundClassVolume(USoundClass* SoundClass, float Volume);
};