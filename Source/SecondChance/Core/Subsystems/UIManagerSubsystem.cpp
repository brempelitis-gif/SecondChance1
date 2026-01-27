#include "UIManagerSubsystem.h"
#include "UI/Base/UIBaseWidget.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "MyGameInstance.h"
#include "NewGameSubsystem.h"
#include "UIActions.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundClass.h"
#include "UI/Settings/AudioOptionType.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Save/AudioSettingsSaveGame.h"
#include "Core/Save/SaveGameSubsystem.h"
#include "GameFramework/GameUserSettings.h"

static const FString AudioSettingsSlot = TEXT("AudioSettings");
static constexpr int32 AudioSettingsUserIndex = 0;

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	CurrentState = EUIState::None;
	CurrentWidget = nullptr;

	// Automātiska UIConfig piesaiste no GameInstance
	if (!UIConfig)
	{
		if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
		{
			UIConfig = GI->UIConfig;
		}
	}
	
	SupportedResolutions = {
		{1920,1080},
		{1600,900},
		{1280,720}
	};
	LoadAudioSettings();
	LoadInitialSettings();
}
void UUIManagerSubsystem::LoadInitialSettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
		// Ielādē pēdējos saglabātos iestatījumus no diska (.ini faila)
		Settings->LoadSettings(true);
		Settings->ApplySettings(false);
	int32 Index = 0;
	
	Settings->GetScreenResolution() == SupportedResolutions[0] ? Index = 0 :
	Settings->GetScreenResolution() == SupportedResolutions[1] ? Index = 1 :
	Settings->GetScreenResolution() == SupportedResolutions[2] ? Index = 2 : 
	PendingResolutionIndex = Index;

	PendingWindowMode = static_cast<int32>(Settings->GetFullscreenMode());
	PendingQuality = Settings->GetOverallScalabilityLevel();
}

void UUIManagerSubsystem::Deinitialize()
{
	HideCurrentUI();
	Super::Deinitialize();
}
EUIState UUIManagerSubsystem::GetCurrentState() const
{
	return CurrentState;
}

// ============================================================
// UI FLOW
// ============================================================
void UUIManagerSubsystem::ShowSplashScreen()
{
	if (!UIConfig || !UIConfig->SplashScreenClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIManagerSubsystem: SplashScreenClass missing"));
		return;
	}
	SetUIState(EUIState::Splash);
	CreateAndShowWidget(UIConfig->SplashScreenClass);

	// 5 sekundes → pāreja uz Main Menu
	if (UWorld* World = GetWorld())
	{
		FTimerHandle SplashTimer;
		World->GetTimerManager().SetTimer(
			SplashTimer,
			this,
			&UUIManagerSubsystem::ShowMainMenu,
			5.f,
			false
		);
	}
}
void UUIManagerSubsystem::ShowMainMenu()
{
	if (!UIConfig || !UIConfig->MainMenuClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIManagerSubsystem: MainMenuClass missing"));
		return;
	}

	SetUIState(EUIState::MainMenu);
	CreateAndShowWidget(UIConfig->MainMenuClass);
}
void UUIManagerSubsystem::ShowOptionsMenu()
{
	if (!UIConfig || !UIConfig->OptionsMenuClass) return;

	SetUIState(EUIState::Options);
}
void UUIManagerSubsystem::ShowPauseMenu()
{
	if (!UIConfig || !UIConfig->PauseMenuClass) return;

	SetUIState(EUIState::Pause);
	CreateAndShowWidget(UIConfig->PauseMenuClass);
}
void UUIManagerSubsystem::HideCurrentUI()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	CurrentState = EUIState::None;
}
void UUIManagerSubsystem::SetUIState(EUIState NewState)
{
	if (CurrentState == NewState)
	{
		return;
	}

	HideCurrentUI();
	CurrentState = NewState;

	switch (CurrentState)
	{
	case EUIState::MainMenu:
		CreateAndShowWidget(UIConfig->MainMenuClass);
		break;

	case EUIState::Options:
		CreateAndShowWidget(UIConfig->OptionsMenuClass);
		break;

	case EUIState::Pause:
		CreateAndShowWidget(UIConfig->PauseMenuClass);
		break;

	default:
		break;
	}
}


void UUIManagerSubsystem::CreateAndShowWidget(TSubclassOf<UUIBaseWidget> WidgetClass)
{
	if (CurrentWidget)
	{
		return; // jau ir aktīvs widgets
	}

	if (!WidgetClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	CurrentWidget = CreateWidget<UUIBaseWidget>(World, WidgetClass);
	if (!CurrentWidget) return;

	CurrentWidget->AddToViewport();
}

// ============================================================
// MENU ACTIONS
// ============================================================
void UUIManagerSubsystem::HandleMenuAction(EUIAction Action)
{
	switch (Action)
	{
	case EUIAction::Resume:
		// TODO: Resume last game
		break;

	case EUIAction::NewGame:
		UE_LOG(LogTemp, Log, TEXT("New Game"));
		// TODO: Start new game
		HandleNewGame();
		break;

	case EUIAction::Load:
		UE_LOG(LogTemp, Log, TEXT("Load Game"));
		// TODO: Show load menu
		HandleLoadGame();
		break;

	case EUIAction::Options:
		ShowOptionsMenu();
		break;

	case EUIAction::Quit:
		if (UWorld* World = GetWorld())
		{
			UKismetSystemLibrary::QuitGame(
				World,
				nullptr,
				EQuitPreference::Quit,
				false
			);
		}
		break;
	}
}
// ============================================================
// NEW GAME
// ============================================================
void UUIManagerSubsystem::HandleNewGame() const
{
	UNewGameSubsystem* NewGameSubsystem =
		GetGameInstance()->GetSubsystem<UNewGameSubsystem>();

	NewGameSubsystem->StartNewGame();
}
// ============================================================
// Load GAME
// ============================================================
void UUIManagerSubsystem::HandleLoadGame() const
{
	USaveGameSubsystem* SaveGameSubsystem =
		GetGameInstance()->GetSubsystem<USaveGameSubsystem>();

	///////////////////////////////////////////////////////////////SaveGameSubsystem->LoadSave(SaveData->);
}
// ============================================================
// AUDIO SETTINGS
// ============================================================
void UUIManagerSubsystem::SetAudioOption(EAudioOption Option, float Value)
{
	switch (Option)
    	{
    	case EAudioOption::Master:
    		PendingMasterVolume = Value;
    		SetMasterVolume(Value); // preview
    		break;
    
    	case EAudioOption::Music:
    		PendingMusicVolume = Value;
    		SetMusicVolume(Value);
    		break;
    
    	case EAudioOption::SFX:
    		PendingSFXVolume = Value;
    		SetSFXVolume(Value);
    		break;
    	}
	
	MarkCategoryPending(ESettingsCategory::Audio);
}
void UUIManagerSubsystem::SetMasterVolume(float Value) const
{
	if (!UIConfig || !UIConfig->MasterSoundClass) return;

	UIConfig->MasterSoundClass->Properties.Volume = Value;
}
void UUIManagerSubsystem::SetMusicVolume(float Value) const
{
	if (!UIConfig || !UIConfig->MusicSoundClass) return;

	UIConfig->MusicSoundClass->Properties.Volume = Value;
}
void UUIManagerSubsystem::SetSFXVolume(float Value) const
{
	if (!UIConfig || !UIConfig->SFXSoundClass) return;

	UIConfig->SFXSoundClass->Properties.Volume = Value;
}
void UUIManagerSubsystem::ApplyAudioSettings()
{
	CurrentMasterVolume = PendingMasterVolume;
	CurrentMusicVolume  = PendingMusicVolume;
	CurrentSFXVolume    = PendingSFXVolume;
	SaveAudioSettings();
	ClearCategoryPending(ESettingsCategory::Audio);
}
void UUIManagerSubsystem::CancelAudioSettings()
{
	PendingMasterVolume = CurrentMasterVolume;
	PendingMusicVolume  = CurrentMusicVolume;
	PendingSFXVolume    = CurrentSFXVolume;

	// Atgriežam skaņu
	SetMasterVolume(CurrentMasterVolume);
	SetMusicVolume(CurrentMusicVolume);
	SetSFXVolume(CurrentSFXVolume);

	ClearCategoryPending(ESettingsCategory::Audio);
}
void UUIManagerSubsystem::SaveAudioSettings() const
{
	UAudioSettingsSaveGame* SaveGame =
		Cast<UAudioSettingsSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UAudioSettingsSaveGame::StaticClass())
		);

	if (!SaveGame) return;

	SaveGame->MasterVolume = CurrentMasterVolume;
	SaveGame->MusicVolume = CurrentMusicVolume;
	SaveGame->SFXVolume = CurrentSFXVolume;

	UGameplayStatics::SaveGameToSlot(
		SaveGame,
		AudioSettingsSlot,
		AudioSettingsUserIndex
	);
}
void UUIManagerSubsystem::LoadAudioSettings()
{
	if (!UGameplayStatics::DoesSaveGameExist(AudioSettingsSlot, AudioSettingsUserIndex))
	{
		return;
	}

	UAudioSettingsSaveGame* SaveGame =
		Cast<UAudioSettingsSaveGame>(
			UGameplayStatics::LoadGameFromSlot(AudioSettingsSlot, AudioSettingsUserIndex)
		);

	if (!SaveGame) return;

	CurrentMasterVolume = SaveGame->MasterVolume;
	CurrentMusicVolume  = SaveGame->MusicVolume;
	CurrentSFXVolume    = SaveGame->SFXVolume;

	// Pending sākumā = Current
	PendingMasterVolume = CurrentMasterVolume;
	PendingMusicVolume  = CurrentMusicVolume;
	PendingSFXVolume    = CurrentSFXVolume;

	// Apply uz audio engine
	SetMasterVolume(CurrentMasterVolume);
	SetMusicVolume(CurrentMusicVolume);
	SetSFXVolume(CurrentSFXVolume);
}


// ============================================================
// GRAPHICS SETTINGS
// ============================================================
void UUIManagerSubsystem::SetResolution(int32 Index)
{
	PendingResolutionIndex = Index;
	MarkCategoryPending(ESettingsCategory::Graphics);
}

void UUIManagerSubsystem::SetWindowMode(int32 Index)
{
	PendingWindowMode = Index;
	MarkCategoryPending(ESettingsCategory::Graphics);
}

void UUIManagerSubsystem::SetGraphicsQuality(int32 Index)
{
	PendingQuality = Index;
	MarkCategoryPending(ESettingsCategory::Graphics);
}
void UUIManagerSubsystem::ApplyGraphicsSettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	if (!Settings) return;

	// Resolution
	Settings->SetScreenResolution(SupportedResolutions[PendingResolutionIndex]);

	// Window Mode
	EWindowMode::Type Mode = EWindowMode::Fullscreen;
	if (PendingWindowMode == 1) Mode = EWindowMode::Windowed;
	if (PendingWindowMode == 2) Mode = EWindowMode::WindowedFullscreen;
	Settings->SetFullscreenMode(Mode);

	// Quality (0–3)
	Settings->SetOverallScalabilityLevel(PendingQuality);

	Settings->ApplySettings(false);
	Settings->SaveSettings();

	// Commit
	CurrentResolutionIndex = PendingResolutionIndex;
	CurrentWindowMode      = PendingWindowMode;
	CurrentQuality         = PendingQuality;

	ClearCategoryPending(ESettingsCategory::Graphics);
}
void UUIManagerSubsystem::CancelGraphicsSettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	if (!Settings) return;
	if (Settings)
	{
		// Atceļ vizuālās izmaiņas un atgriež pēdējās saglabātās
		Settings->LoadSettings(true);
		Settings->ApplySettings(false);
	}
	/*
	
	Settings->SetScreenResolution(SupportedResolutions[CurrentResolutionIndex]);

	// Restore window mode
	EWindowMode::Type Mode = EWindowMode::Fullscreen;
	if (CurrentWindowMode == 1) Mode = EWindowMode::Windowed;
	if (CurrentWindowMode == 2) Mode = EWindowMode::WindowedFullscreen;
	Settings->SetFullscreenMode(Mode);

	// Restore quality
	Settings->SetOverallScalabilityLevel(CurrentQuality);
	//Settings->ApplySettings(false);
	Settings->ApplySettings(true); // true = saglabāt diskā uzreiz
	// Restore pending
	*/
	PendingResolutionIndex = CurrentResolutionIndex;
	PendingWindowMode      = CurrentWindowMode;
	PendingQuality         = CurrentQuality;

	ClearCategoryPending(ESettingsCategory::Graphics);
}

// ============================================================
// CONTROLS SETTINGS (future)
// ============================================================
bool UUIManagerSubsystem::IsCategoryPending(ESettingsCategory Category) const
{
	return PendingCategories.Contains(Category);
}
/*
// SetKeyBinding()
void UUIManagerSubsystem::ApplyPendingSettings()
{
	UGameUserSettings* Settings = GEngine->GetGameUserSettings();
	
	if (Settings)
	{
		// 1. Apstiprina grafikas izmaiņas (rezolūcija, FPS limit, Scale)
		if (PendingCategories.Contains(ESettingsCategory::Graphics))
		{
			Settings->ApplySettings(true); // true = saglabāt diskā uzreiz
		}
		
		// 2. Audio un citu kategoriju saglabāšana (ja izmanto citu sistēmu)
		// It kā SaveAudioSettings();
	}

	PendingCategories.Empty();
}
void UUIManagerSubsystem::ApplyPendingSettings()
{
	// Ja ir Graphics pending → PRASĀM confirm
	if (PendingCategories.Contains(ESettingsCategory::Graphics))
	{
		if (!OnConfirmSettingsRequired.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("Confirm popup not bound, applying anyway"));
			ApplyPendingSettings_Internal();
			return;
		}

		OnConfirmSettingsRequired.Broadcast(ESettingsCategory::Graphics);
		return;
	}

	// Citādi var droši apply uzreiz
	ApplyPendingSettings_Internal();
}

*/
void UUIManagerSubsystem::ApplyPendingSettings()
{
	TArray<ESettingsCategory> Categories = PendingCategories.Array();

	for (ESettingsCategory Category : Categories)
	{
		switch (Category)
		{
		case ESettingsCategory::Audio:
			ApplyAudioSettings();
			break;

		case ESettingsCategory::Graphics:
			ApplyGraphicsSettings();
			break;

		case ESettingsCategory::Controls:
			// ApplyControlsSettings();
			break;

		case ESettingsCategory::Gameplay:
			// ApplyGameplaySettings();
			break;

		default:
			break;
		}
	}

	PendingCategories.Empty();

	// Notify UI (refresh buttons etc.)
	OnSettingsChanged.Broadcast(ESettingsCategory::None);
}

void UUIManagerSubsystem::CancelPendingSettings()
{
	TArray<ESettingsCategory> Categories = PendingCategories.Array();
	
	for (ESettingsCategory Category : Categories)
	{
		switch (Category)
		{
		case ESettingsCategory::Audio:
			CancelAudioSettings();
			break;

		case ESettingsCategory::Graphics:
			CancelGraphicsSettings();
			break;

		case ESettingsCategory::Controls:
			//CancelControlsSettings();
			break;
		default: ;
		}
	}
	PendingCategories.Empty();
	OnSettingsChanged.Broadcast(ESettingsCategory::None);
}

// ============================================================
// ACCESSIBILITY SETTINGS (future)
// ============================================================

// UIScale
// ColorBlindMode
// Subtitles


