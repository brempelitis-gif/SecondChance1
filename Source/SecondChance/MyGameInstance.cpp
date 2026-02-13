#include "MyGameInstance.h"

#include "Core/Save/SaveIndex.h"
#include "Core/Save/SCSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Framework/Application/SlateApplication.h"
#include "Core/Subsystems/UIManagerSubsystem.h"

class USaveIndex;

void UMyGameInstance::Init()
{
	Super::Init();

	if (UUIManagerSubsystem* UIManager = GetSubsystem<UUIManagerSubsystem>())
	{
		// Piesaistām Config
		UIManager->UIConfig = UIConfig;
	}
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
}

UUIManagerSubsystem* UMyGameInstance::GetUIManager() const
{
	return GetSubsystem<UUIManagerSubsystem>();
}

void UMyGameInstance::AsyncLoadGameLevel(FName LevelName)
{
	// 1. Parādām Splash Screen (šeit tu izsauc savu UI sistēmu)
	if (SplashScreenClass)
	{
		UUserWidget* Splash = CreateWidget<UUserWidget>(this, SplashScreenClass);
		if (Splash)
		{
			if (UUIManagerSubsystem* UIManager = GetSubsystem<UUIManagerSubsystem>())
			{
				UIManager->PushWidget(Splash);
			}
		}
	}
	// 2. Ceļa validācija un ielāde
	// Pārliecināmies, ka ceļš sākas ar /Game/
	FString LevelPath = LevelName.ToString();
	if (!LevelPath.StartsWith(TEXT("/Game/")))
	{
		// Pielāgo šo ceļu savai mapju struktūrai!
		LevelPath = TEXT("/Game/ManaSpele/Levels/") + LevelPath;
	}

	LoadPackageAsync(LevelPath,
		FLoadPackageAsyncDelegate::CreateLambda([this, LevelPath](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		{
			if (Result == EAsyncLoadingResult::Succeeded)
			{
				// Kad ielādēts, izmantojam nosaukumu bez ceļa priekš OpenLevel
				FName ShortLevelName = FName(*FPackageName::GetShortName(PackageName));
				UGameplayStatics::OpenLevel(this, ShortLevelName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("AsyncLoading: Neizdevās ielādēt %s"), *LevelPath);
			}
		}),
		0, PKG_ContainsMap);
}

void UMyGameInstance::OnLevelLoaded()
{
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
        
        // Meklējam vecāko datumu
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

        // Dabūjam slota nosaukumu, ko pārrakstīt (piem. "SaveSlot_03")
        TargetSlotName = IndexSave->SavedGames[OldestIndex].SlotName;

        UE_LOG(LogTemp, Warning, TEXT("Sasniegts limits! Pārrakstām vecāko seivu: %s (Datums: %s)"), 
            *TargetSlotName, *OldestDate.ToString());

        // Izņemam veco ierakstu no saraksta (pievienosim jaunu beigās)
        IndexSave->SavedGames.RemoveAt(OldestIndex);
    }
    else
    {
        // --- SCENĀRIJS B: Veidojam jaunu slotu ---
        // Ģenerējam unikālu nosaukumu, piemēram, ar GUID vai vienkāršu Timestamp
        TargetSlotName = "SaveSlot_" + FString::FromInt(FDateTime::Now().ToUnixTimestamp());
    }

    // 3. Saglabājam PAŠU SPĒLI (Heavy Data)
    USCSaveGame* NewSave = Cast<USCSaveGame>(UGameplayStatics::CreateSaveGameObject(USCSaveGame::StaticClass()));
    if (NewSave)
    {
        NewSave->PlayerData = CharacterData;
        NewSave->SaveSlotName = TargetSlotName;
        NewSave->CreationDate = FDateTime::Now(); // Ierakstām pašreizējo laiku

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
	
	// Funkcijas beigās atgriežam izmantoto TargetSlotName
	return TargetSlotName;
}
