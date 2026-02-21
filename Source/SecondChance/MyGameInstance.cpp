#include "MyGameInstance.h"
#include "Core/Save/SaveIndex.h"
#include "Core/Save/SCSaveGame.h"
#include "Core/Subsystems/UIConfig.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "UI/Base/SplashScreen/Splash.h"
#include "TimerManager.h"
#include "Engine/World.h"

class USaveIndex;

void UMyGameInstance::Init()
{
    Super::Init();
    
    UIMan = GetSubsystem<UUIManagerSubsystem>();
    if (UIMan) UIMan->UIConfig = UIConfig; 
}
void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
    // Šī ir dzinēja funkcija, kas izsaucas, kad ielāde IR pabeigta
    Super::LoadComplete(LoadTime, MapName);
    
    UE_LOG(LogTemp, Log, TEXT("GameInstance: Ielāde pabeigta mapei: %s"), *MapName);
    OnLevelLoaded();
}

void UMyGameInstance::Shutdown()
{
    Super::Shutdown();
}

UUIManagerSubsystem* UMyGameInstance::GetUIManager() const
{
    return GetSubsystem<UUIManagerSubsystem>();
}

void UMyGameInstance::TogglePauseMenu()
{
    if (!UIMan || !UIConfig || !UIConfig->PauseMenuClass) return;

    // 1. Pārbaudām, vai staks nav tukšs un vai pēdējais logs ir Pauzes izvēlne
    if (UIMan->WidgetStack.Num() > 0 && UIMan->WidgetStack.Last()->IsA(UIConfig->PauseMenuClass))
    {
       // Ja pauze jau ir virspusē, aizveram to
       UIMan->PopWidget();
        
       // Atpauzējam spēli (tavs PopWidget to izdara automātiski, ja staks kļūst tukšs, 
       // bet drošības pēc, ja ir citi logi apakšā:)
       UGameplayStatics::SetGamePaused(GetWorld(), false);
    }
    else
    {
       // 2. Ja pauze nav atvērta, izveidojam un "uzstumjam" to
       UUserWidget* PauseMenu = CreateWidget<UUserWidget>(this, UIConfig->PauseMenuClass);
       if (PauseMenu)
       {
          // Izmantojam tavu PushWidget: bShowCursor = true, bPauseGame = true
          UIMan->PushWidget(PauseMenu, true, true);
       }
    }
}

void UMyGameInstance::PrepareForLoad(FString SlotName)
{
    CurrentSlotToLoad = SlotName;
    LastCapturedPortraitName = SlotName; // Šis nodrošina bildi HUD portretā
    bIsLoadingFromSave = true;

    UE_LOG(LogTemp, Log, TEXT("GameInstance: Sagatavota ielāde slotam: %s"), *SlotName);
}

void UMyGameInstance::AsyncLoadGameLevel(FName LevelName)
{
    // 1. Parādām Splash Screen
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->SplashWidgetClass)
    {
        UUserWidget* SplasheWidget = CreateWidget<UUserWidget>(GetWorld(), UIMan->UIConfig->SplashWidgetClass);
        if (SplasheWidget)
        {
            // --- LABOJUMS ŠEIT ---
            // Padodam 'false, false', lai pele nerādītos un, GALVENAIS, lai spēle netiktu nopauzēta!
            // Ja spēle apstājas, taimeris nekad neizsauks OpenLevel.
            UIMan->PushWidget(SplasheWidget, false, false);
        }
    }

    // 2. Ja nosaukumā ir pilns ceļš, izvelkam tikai īso nosaukumu
    FString LevelPath = LevelName.ToString();
    if (LevelPath.StartsWith(TEXT("/Game/")))
    {
        LevelName = FName(*FPaths::GetBaseFilename(LevelPath));
    }

    // 3. Dodam mirkli UI uzzīmēties un tad veram vaļā līmeni
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, LevelName]()
    {
        UGameplayStatics::OpenLevel(this, LevelName);
    }, 0.1f, false);
}

void UMyGameInstance::UpdateSaveIndex(FString SlotName, FString PlayerName)
{
}



void UMyGameInstance::OnLevelLoaded()
{
    if (UIMan)
    {
        // Tīrām TIKAI Splash logrīkus, nevis visu Viewportu
        for (int32 i = UIMan->WidgetStack.Num() - 1; i >= 0; --i)
        {
            UUserWidget* CurrentWidget = UIMan->WidgetStack[i];
            // Ja tas ir Splash klases logrīks, tad metam ārā
            if (CurrentWidget && CurrentWidget->IsA(UIConfig->SplashWidgetClass))
            {
                UIMan->WidgetStack.RemoveAt(i);
                CurrentWidget->RemoveFromParent();
            }
        }
        
        // Atjaunojam Input Mode uz spēli, lai poga "P" strādātu
        UIMan->UpdateInputMode(); 
    }
}

FString UMyGameInstance::CreateNewSaveGame(FCharacterCustomizationData CharacterData)
{
    // 1. Ielādējam vai izveidojam Indeksu
    USaveIndex* IndexSave = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot(MASTER_SAVE_INDEX, 0));
    if (!IndexSave)
    {
        IndexSave = Cast<USaveIndex>(UGameplayStatics::CreateSaveGameObject(USaveIndex::StaticClass()));
    }

    FString TargetSlotName;

    // 2. PĀRBAUDE: Vai sasniegts limits (10)?
    if (IndexSave->SavedGames.Num() >= 10)
    {
        // --- SCENĀRIJS A: Pārrakstām vecāko ---
        int32 OldestIndex = 0;
        FDateTime OldestDate = FDateTime::MaxValue();

        for (int32 i = 0; i < IndexSave->SavedGames.Num(); i++)
        {
            if (IndexSave->SavedGames[i].SaveDate < OldestDate)
            {
                OldestDate = IndexSave->SavedGames[i].SaveDate;
                OldestIndex = i;
            }
        }

        TargetSlotName = IndexSave->SavedGames[OldestIndex].SlotName;

        UE_LOG(LogTemp, Warning, TEXT("Sasniegts limits! Pārrakstām vecāko seivu: %s (Datums: %s)"), 
            *TargetSlotName, *OldestDate.ToString());

        IndexSave->SavedGames.RemoveAt(OldestIndex);
    }
    else
    {
        // --- SCENĀRIJS B: Veidojam jaunu slotu ---
        TargetSlotName = "SaveSlot_" + FString::FromInt(FDateTime::Now().ToUnixTimestamp());
    }

    // 3. Saglabājam PAŠU SPĒLI (Heavy Data)
    USCSaveGame* NewSave = Cast<USCSaveGame>(UGameplayStatics::CreateSaveGameObject(USCSaveGame::StaticClass()));
    if (NewSave)
    {
        NewSave->PlayerData = CharacterData;
        NewSave->SaveSlotName = TargetSlotName;
        NewSave->CreationDate = FDateTime::Now(); 

        UGameplayStatics::SaveGameToSlot(NewSave, TargetSlotName, 0);
    }

    // 4. Atjaunojam INDEKSU (Light Data)
    FSaveMetadata NewMeta;
    NewMeta.SlotName = TargetSlotName;
    NewMeta.PlayerName = CharacterData.PlayerName;
    NewMeta.SaveDate = FDateTime::Now();

    IndexSave->SavedGames.Add(NewMeta);
    
    // Saglabājam indeksa failu
    UGameplayStatics::SaveGameToSlot(IndexSave, MASTER_SAVE_INDEX, 0);

    UE_LOG(LogTemp, Log, TEXT("Jauna spēle veiksmīgi saglabāta slotā: %s"), *TargetSlotName);
    
    return TargetSlotName;
}

void UMyGameInstance::ClearLoadData()
{
    CurrentSlotToLoad = TEXT("");
    bIsLoadingFromSave = false;
}