#include "MainMenuWidget.h"

#include "MyGameInstance.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Core/Save/SaveIndex.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Menus/LoadGame/LoadGameMenuWidget.h"
#include "UI/Menus/Options/UIOptionsMenuBase.h"

class UMyGameInstance;

void UMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    BindButtons();

    // Sākumā "Continue" poga varētu būt izslēgta, ja nav saglabātu spēļu
    // Te varētu pielikt pārbaudi: ContinueButton->SetIsEnabled(HasSaveGame());
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
    
    // Katru reizi, kad logrīks tiek parādīts, pārbaudām seivus
    RefreshSaveAvailability();
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
    // 1. Ielādējam indeksu
    USaveIndex* IndexSave = Cast<USaveIndex>(UGameplayStatics::LoadGameFromSlot(TEXT("MasterSaveIndex"), 0));
    
    if (IndexSave && IndexSave->SavedGames.Num() > 0)
    {
        // 2. Atrodam jaunāko seivu (salīdzinām datumus)
        FSaveMetadata LatestSave = IndexSave->SavedGames[0];
        
        for (const FSaveMetadata& Meta : IndexSave->SavedGames)
        {
            if (Meta.SaveDate > LatestSave.SaveDate)
            {
                LatestSave = Meta;
            }
        }

        // 3. Sagatavojam GameInstance ielādei
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        if (GI)
        {
            // Šeit mēs izmantojam tos mainīgos, ko iepriekš sarunājām pievienot GI
            GI->CurrentSlotToLoad = LatestSave.SlotName;
            GI->bIsLoadingFromSave = true;

            UE_LOG(LogTemp, Log, TEXT("Continue: Ielādējam jaunāko slotu: %s"), *LatestSave.SlotName);
            
            // 4. Sākam ielādi
            GI->AsyncLoadGameLevel(FName("L_GameLevel"));
        }
    }
}

void UMainMenuWidget::HandleLoadGameClicked()
{
    UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->LoadGameMenuClass)
    {
        // Izveidojam logrīku
        ULoadGameMenuWidget* LoadGameWidget = CreateWidget<ULoadGameMenuWidget>(GetWorld(), UIMan->UIConfig->LoadGameMenuClass);
        if (LoadGameWidget)
        {
            // Uzstumjam to uz ekrāna virs Main Menu
            UIMan->PushWidget(LoadGameWidget);
        }
    }
}

void UMainMenuWidget::HandleNewGameClicked()
{
    // Izmantojam asinhrono ielādi no GameInstance, lai parādītu Splash Screen un ielādētu nākamo līmeni

    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
    	GI->AsyncLoadGameLevel(FName("L_CharacterSetup"));
    }
    
}

void UMainMenuWidget::HandleOptionsClicked()
{
    UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    if (UIMan && UIMan->UIConfig && UIMan->UIConfig->OptionsMenuClass)
    {
        // Izveidojam logrīku
        UUIOptionsMenuBase* OptionsWidget = CreateWidget<UUIOptionsMenuBase>(GetWorld(), UIMan->UIConfig->OptionsMenuClass);
        if (OptionsWidget)
        {
            // Uzstumjam to uz ekrāna virs Main Menu
            UIMan->PushWidget(OptionsWidget);
        }
    }
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
