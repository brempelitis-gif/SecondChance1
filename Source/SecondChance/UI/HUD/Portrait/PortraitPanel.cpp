#include "UI/HUD/Portrait/PortraitPanel.h"
#include "Components/Image.h"
#include "Kismet/KismetRenderingLibrary.h" // Nepieciešams modulim "RenderCore"
#include "MyGameInstance.h"

void UPortraitPanel::NativeConstruct()
{
	Super::NativeConstruct();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && Image_PlayerPortrait)
	{
		// Izveidojam ceļu līdz bildei
		FString FullPath = FPaths::ProjectSavedDir() + "Screenshots/Windows/" + GI->LastCapturedPortraitName + ".png";
        
		UTexture2D* LoadedTex = UKismetRenderingLibrary::ImportFileAsTexture2D(this, FullPath);
		if (LoadedTex)
		{
			Image_PlayerPortrait->SetBrushFromTexture(LoadedTex);
		}
	}
}