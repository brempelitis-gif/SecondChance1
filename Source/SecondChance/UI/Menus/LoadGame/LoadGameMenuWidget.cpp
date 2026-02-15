#include "UI/Menus/LoadGame/LoadGameMenuWidget.h"
#include "HAL/PlatformFileManager.h" // Nepieciešams failu dzēšanai
#include "Components/ScrollBox.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "UI/Menus/LoadGame/SaveSlotWidget.h"
#include "Core/Save/SaveIndex.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULoadGameMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BindButtons();
}
void ULoadGameMenuWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (BackBtn) BackBtn->SetLabel(BackBtnLabel);
    if (DeleteBtn) DeleteBtn->SetLabel(DeleteBtnLabel);
    if (LoadBtn) LoadBtn->SetLabel(LoadBtnLabel);
}
void ULoadGameMenuWidget::NativeConstruct()
{
    // 1. Paslēpjam pogas sākumā
    if (DeleteBtn) DeleteBtn->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadBtn) LoadBtn->SetVisibility(ESlateVisibility::Collapsed);

    // 2. Notīrām iepriekšējo izvēli
    SelectedSlotName = "";

    RefreshSaveList();
}

void ULoadGameMenuWidget::BindButtons()
{
    if (BackBtn) BackBtn->OnClicked.AddDynamic(this, &ULoadGameMenuWidget::HandleBackClicked);
    if (DeleteBtn) DeleteBtn->OnClicked.AddDynamic(this, &ULoadGameMenuWidget::HandleDeleteClicked);
    if (LoadBtn) LoadBtn->OnClicked.AddDynamic(this, &ULoadGameMenuWidget::HandleLoadClicked);
}

void ULoadGameMenuWidget::RefreshSaveList()
{
    if (!SaveListScrollBox || !SaveSlotWidgetClass) return;

    // 1. Notīrām vecos slotus no saraksta
    SaveListScrollBox->ClearChildren();

    // 2. Ielādējam Master Index
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    USaveIndex* IndexSave = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot("MasterSaveIndex", 0));

    if (!IndexSave || IndexSave->SavedGames.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadMenu: Nav atrasts neviens seivs."));
        return;
    }

    // 3. KĀRTOŠANA: Jaunākie seivi augšgalā
    IndexSave->SavedGames.Sort([](const FSaveMetadata& A, const FSaveMetadata& B) {
        return A.SaveDate > B.SaveDate;
    });

    // 4. Veidojam slotus katram ierakstam
    for (const FSaveMetadata& Meta : IndexSave->SavedGames)
    {
        USaveSlotWidget* NewSlot = CreateWidget<USaveSlotWidget>(this, SaveSlotWidgetClass);
        if (NewSlot)
        {
            NewSlot->SetupSlot(Meta);
            
            // Piesaistām klikšķa notikumu
            NewSlot->OnSlotSelected.AddDynamic(this, &ULoadGameMenuWidget::HandleSlotSelected);
            
            // Pievienojam ScrollBox
            SaveListScrollBox->AddChild(NewSlot);
        }
    }
}

void ULoadGameMenuWidget::HandleSlotSelected(FString SlotName)
{
    SelectedSlotName = SlotName;
    UE_LOG(LogTemp, Log, TEXT("Slots izvēlēts: %s. Aktivizējam darbības."), *SlotName);

    // Parādām pogas
    if (DeleteBtn) DeleteBtn->SetVisibility(ESlateVisibility::Visible);
    if (LoadBtn) LoadBtn->SetVisibility(ESlateVisibility::Visible);
}

void ULoadGameMenuWidget::HandleDeleteClicked()
{
    if (SelectedSlotName.IsEmpty()) return;

    // 1. Izdzēšam pašu SaveGame failu no Unreal sistēmas
    UGameplayStatics::DeleteGameInSlot(SelectedSlotName, 0);

    // 2. Izdzēšam Screenshot failu no diska
    FString ScreenshotPath = FPaths::ProjectSavedDir() + "Screenshots/WindowsEditor/" + SelectedSlotName + "_Portrait.png";
    if (FPaths::FileExists(ScreenshotPath))
    {
        IFileManager::Get().Delete(*ScreenshotPath);
    }

    // 3. Izņemam ierakstu no Master Index
    USaveIndex* IndexSave = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot("MasterSaveIndex", 0));
    if (IndexSave)
    {
        // Atrodam un izņemam ierakstu no masīva
        for (int32 i = 0; i < IndexSave->SavedGames.Num(); i++)
        {
            if (IndexSave->SavedGames[i].SlotName == SelectedSlotName)
            {
                IndexSave->SavedGames.RemoveAt(i);
                break;
            }
        }
        // Saglabājam atjaunoto indeksu atpakaļ diskā
        UGameplayStatics::SaveGameToSlot(IndexSave, "MasterSaveIndex", 0);
    }

    // 4. Atjaunojam UI
    SelectedSlotName = "";
    if (DeleteBtn) DeleteBtn->SetVisibility(ESlateVisibility::Collapsed);
    if (LoadBtn) LoadBtn->SetVisibility(ESlateVisibility::Collapsed);
    
    RefreshSaveList(); // Pārzīmējam sarakstu (izdzēstais slots pazudīs)

    UE_LOG(LogTemp, Warning, TEXT("Slots %s un tā fails ir izdzēsti."), *SelectedSlotName);
}

void ULoadGameMenuWidget::HandleLoadClicked()
{
    if (SelectedSlotName.IsEmpty()) return;

    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
        // Šeit mēs pasakām GameInstance, kuru slotu ielādēt pēc līmeņa ielādes
        // Pieņemsim, ka tev GameInstance ir mainīgais 'CurrentSlotToLoad'
        GI->CurrentSlotToLoad = SelectedSlotName;

        GI->AsyncLoadGameLevel(FName("L_GameLevel"));
    }
}

void ULoadGameMenuWidget::HandleBackClicked()
{
    // Atgriežamies uz galveno izvēlni (L_MainMenu)
    UGameplayStatics::OpenLevel(this, FName("L_MainMenu"));
}
