#include "MainMenuWidget.h"

#include "MyGameInstance.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
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
    if (NewGameButton)  NewGameButton->SetLabel(NewGameLabel);
    if (OptionsButton)  OptionsButton->SetLabel(OptionsLabel);
    if (QuitButton)     QuitButton->SetLabel(QuitLabel);
}

void UMainMenuWidget::BindButtons()
{
    if (ContinueButton) ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleContinueClicked);
    if (NewGameButton)  NewGameButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleNewGameClicked);
    if (OptionsButton)  OptionsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleOptionsClicked);
    if (QuitButton)     QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HandleQuitClicked);
}

void UMainMenuWidget::HandleContinueClicked()
{
    // TODO: Ielādēt pēdējo saglabāto spēli
    UE_LOG(LogTemp, Log, TEXT("Continue Game Pressed"));
}

void UMainMenuWidget::HandleNewGameClicked()
{
    // Izmantojam asinhrono ielādi no GameInstance, lai parādītu Splash Screen un ielādētu nākamo līmeni

    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
    	GI->AsyncLoadGameLevel(FName("L_CharacterSetup"));
    }
    //UGameplayStatics::OpenLevel(this, FName("/Game/ManaSpele/Levels/L_CharacterSetup"));
    
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
