#include "UI/Menus/LoadGame/SaveSlotWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Kismet/KismetRenderingLibrary.h" // PIEVIENO ŠO
#include "HAL/FileManager.h"              // PIEVIENO ŠO

void USaveSlotWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BindButtons();
}

void USaveSlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (SelectBtn) SelectBtn->SetLabel(SelectBtnLabel);
}

void USaveSlotWidget::BindButtons()
{
    if (SelectBtn) SelectBtn->OnClicked.AddDynamic(this, &USaveSlotWidget::HandleBtnClicked);
}

void USaveSlotWidget::SetupSlot(const FSaveMetadata& Metadata)
{
    CachedSlotName = Metadata.SlotName;

    if (PlayerNameText) PlayerNameText->SetText(FText::FromString(Metadata.PlayerName));
    if (SaveDateText) SaveDateText->SetText(FText::FromString(Metadata.SaveDate.ToString(TEXT("%d.%m.%Y %H:%M"))));

    // 1. Izveidojam bāzes ceļu līdz Screenshots mapei
    FString ScreensDir = FPaths::ProjectSavedDir() + "Screenshots/";

    // 2. Definējam iespējamos variantus (Editoram un Fināla versijai)
    FString PathEditor = ScreensDir + "WindowsEditor/" + Metadata.SlotName + "_Full.png";
    FString PathBuild = ScreensDir + "Windows/" + Metadata.SlotName + "_Full.png";

    FString FinalPath = "";

    // 3. Pārbaudām, kurš fails reāli eksistē
    if (IFileManager::Get().FileExists(*PathEditor))
    {
        FinalPath = PathEditor;
    }
    else if (IFileManager::Get().FileExists(*PathBuild))
    {
        FinalPath = PathBuild;
    }

    if (!FinalPath.IsEmpty())
    {
        // Pārvēršam par pilno ceļu drošībai
        FString AbsolutePath = FPaths::ConvertRelativePathToFull(FinalPath);
        
        UTexture2D* LoadedFull = LoadTextureFromFile(AbsolutePath);
        if (LoadedFull && PortraitImage)
        {
            PortraitImage->SetBrushFromTexture(LoadedFull);
            UE_LOG(LogTemp, Log, TEXT("Bilde ielādēta no: %s"), *AbsolutePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Bilde netika atrasta nevienā mapē priekš slota: %s"), *Metadata.SlotName);
    }
}

void USaveSlotWidget::HandleBtnClicked()
{
    OnSlotSelected.Broadcast(CachedSlotName);
}

// JAUNĀ UN UZLABOTĀ VERSIJA
UTexture2D* USaveSlotWidget::LoadTextureFromFile(const FString& FullPath)
{
    if (!FPaths::FileExists(FullPath)) 
    {
        UE_LOG(LogTemp, Error, TEXT("Fails neeksistē: %s"), *FullPath);
        return nullptr;
    }

    // Aizmirsti par ImageWrapper un BulkData. Lock. 
    // Šī viena rindiņa izdara visu tavā vietā:
    return UKismetRenderingLibrary::ImportFileAsTexture2D(GetWorld(), FullPath);
}