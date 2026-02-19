#include "UI/HUD/Portrait/PortraitPanel.h"
#include "Components/Image.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MyGameInstance.h"
#include "HAL/FileManager.h"

void UPortraitPanel::NativeConstruct()
{
	Super::NativeConstruct();

	// 1. Vispirms uzliekam noklusēto siluetu
	if (Image_PlayerPortrait && DefaultPortrait)
	{
		Image_PlayerPortrait->SetBrushFromTexture(DefaultPortrait);
	}

	// 2. Ieliekam mazu aizturi, lai cietais disks paspēj "ievilkt elpu" pēc screenshot uzņemšanas
	FTimerHandle LoadDelayTimer;
	GetWorld()->GetTimerManager().SetTimer(LoadDelayTimer, [this]()
	{
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		if (!GI || !Image_PlayerPortrait) return;

		// SkillTree logā mēs izmantojām UniqueSlotID, tāpēc meklējam to
		// Ja UniqueSlotID nav pieejams, mēģinām vārdu (pārbaudi, kā tu saglabāji SkillTree)
		FString FileName = GI->FinalCharacterData.PlayerName + "_Face.png";
        
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
	}, 0.2f, false);
}