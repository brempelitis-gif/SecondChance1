#include "MainMenuWidget.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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
    // Piemērs: Ielādējam pirmo līmeni
    UGameplayStatics::OpenLevel(this, FName("L_StarterLevel"));
}

void UMainMenuWidget::HandleOptionsClicked()
{
    UUIManagerSubsystem* UIMan = GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
    
    // Ja klase ir iestatīta, izveidojam logrīku un "uzstumjam" to uz steka
    if (UIMan && OptionsMenuClass)
    {
        UUserWidget* OptionsWidget = CreateWidget<UUserWidget>(this, OptionsMenuClass);
        if (OptionsWidget)
        {
            UIMan->PushWidget(OptionsWidget);
        }
    }
}

void UMainMenuWidget::HandleQuitClicked()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
