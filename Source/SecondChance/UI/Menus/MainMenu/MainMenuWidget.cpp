#include "MainMenuWidget.h"

#include "MyGameInstance.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Core/Save/SaveIndex.h"
#include "Core/Subsystems/UIConfig.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Menus/LoadGame/LoadGameMenuWidget.h"
#include "UI/Menus/Options/UIOptionsMenuBase.h"

class UMyGameInstance;

void UMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BindButtons();
}

void UMainMenuWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (ContinueButton) ContinueButton->SetLabel(ContinueLabel);
    if (LoadGameButton)  LoadGameButton->SetLabel(LoadGameLabel);
    if (NewGameButton)  NewGameButton->SetLabel(NewGameLabel);
    if (OptionsButton)  OptionsButton->SetLabel(OptionsLabel);
    if (QuitButton)     QuitButton->SetLabel(QuitLabel);
}
void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // Inicializējam atsauces vienreiz
    GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
        UIMan = GI->GetSubsystem<UUIManagerSubsystem>();
    }
    // Katru reizi, kad logrīks tiek parādīts, pārbaudām seivus
    RefreshSaveAvailability();
}
// Jauna, optimizēta funkcija apakšizvēlņu atvēršanai
void UMainMenuWidget::OpenSubMenu(TSubclassOf<UUserWidget> MenuClass)
{
    if (UIMan && MenuClass)
    {
        UUserWidget* NewMenu = CreateWidget<UUserWidget>(this, MenuClass);
        if (NewMenu)
        {
            UIMan->PushWidget(NewMenu);
        }
    }
}
void UMainMenuWidget::BindButtons()
{
    if (ContinueButton) ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleContinueClicked);
    if (LoadGameButton)  LoadGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleLoadGameClicked);
    if (NewGameButton)  NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleNewGameClicked);
    if (OptionsButton)  OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleOptionsClicked);
    if (QuitButton)     QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuitClicked);
}

void UMainMenuWidget::HandleContinueClicked()
{
    USaveIndex* IndexSave = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot(TEXT("MasterSaveIndex"), 0));
    
    if (IndexSave && IndexSave->SavedGames.Num() > 0)
    {
        // Atrodam jaunāko
        FSaveMetadata LatestSave = IndexSave->SavedGames[0];
        for (const FSaveMetadata& Meta : IndexSave->SavedGames)
        {
            if (Meta.SaveDate > LatestSave.SaveDate) LatestSave = Meta;
        }

        if (GI)
        {
            // Izmantojam jauno helper funkciju
            GI->PrepareForLoad(LatestSave.SlotName);
            GI->AsyncLoadGameLevel(FName("L_GameLevel"));
        }
    }
}

void UMainMenuWidget::HandleLoadGameClicked()
{
    if (UIMan && UIMan->UIConfig) OpenSubMenu(UIMan->UIConfig->LoadGameMenuClass);
}

void UMainMenuWidget::HandleNewGameClicked()
{
    if (GI)  GI->AsyncLoadGameLevel(FName("L_CharacterSetup"));
}

void UMainMenuWidget::HandleOptionsClicked()
{
    if (UIMan && UIMan->UIConfig) OpenSubMenu(UIMan->UIConfig->OptionsMenuClass);
}

void UMainMenuWidget::HandleQuitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::RefreshSaveAvailability()
{
    // 1. Mēģinām ielādēt indeksu
    USaveIndex* IndexSave = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot(TEXT("MasterSaveIndex"), 0));
    
    // 2. Noskaidrojam, vai ir vismaz viens seivs
    bool bHasSaves = (IndexSave != nullptr && IndexSave->SavedGames.Num() > 0);

    // 3. Iestatām redzamību. Ja nav seivu - Collapsed (neaizņem vietu) vai Hidden
    ESlateVisibility SaveVisibility = bHasSaves ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;

    if (ContinueButton) ContinueButton->SetVisibility(SaveVisibility);
    if (LoadGameButton) LoadGameButton->SetVisibility(SaveVisibility);
}
