#include "UI/HUD/Portrait/PortraitPanel.h"
#include "Components/Image.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MyGameInstance.h"
#include "HAL/FileManager.h"

void UPortraitPanel::NativeConstruct()
{
	Super::NativeConstruct();

	if (Image_PlayerPortrait && DefaultPortrait)
	{
		Image_PlayerPortrait->SetBrushFromTexture(DefaultPortrait);
	}

	FTimerHandle LoadDelayTimer;
	// Palielinām aizturi uz 0.5s, lai drošāk sagaidītu, ka HDD pabeidz rakstīt failu
	GetWorld()->GetTimerManager().SetTimer(LoadDelayTimer, [this]()
	{
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		if (!GI || !Image_PlayerPortrait) return;

		// --- LABOJUMS ŠEIT ---
		// Izmantojam ID, ko saglabājām SkillTree logā, nevis PlayerName
		FString FileName = GI->LastCapturedPortraitName + "_Face.png";
        
		UE_LOG(LogTemp, Warning, TEXT("DEBUG PORTRAIT: Meklēju failu: %s"), *FileName);

		FString SavedDir = FPaths::ProjectSavedDir() + "Screenshots/";
		FString PathEditor = SavedDir + "WindowsEditor/" + FileName;
		FString PathStandalone = SavedDir + "Windows/" + FileName;

		FString FinalPath = "";
		if (IFileManager::Get().FileExists(*PathEditor)) FinalPath = PathEditor;
		else if (IFileManager::Get().FileExists(*PathStandalone)) FinalPath = PathStandalone;

		if (!FinalPath.IsEmpty())
		{
			UTexture2D* LoadedTex = UKismetRenderingLibrary::ImportFileAsTexture2D(this, FinalPath);
			if (LoadedTex)
			{
				Image_PlayerPortrait->SetBrushFromTexture(LoadedTex);
				UE_LOG(LogTemp, Log, TEXT("PORTRAIT: Veiksmīgi ielādēts no %s"), *FinalPath);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("DEBUG PORTRAIT: Fails netika atrasts! Pārbaudīts: %s"), *PathEditor);
		}
	}, 0.5f, false);
}