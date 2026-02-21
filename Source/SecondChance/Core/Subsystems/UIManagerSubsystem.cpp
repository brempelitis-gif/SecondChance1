
#include "Core/Subsystems/UIManagerSubsystem.h"
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Subsystems/UIConfig.h" 
#include "UI/Base/ConfirmationPopUp/UIConfirmationPopup.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    // Mēģinām atrast UIConfig caur GameInstance, ja tas nav iestatīts defaultos
    if (!UIConfig)
    {
        if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
        {
            UIConfig = GI->UIConfig;
        }
    }
}

UUIConfirmationPopup* UUIManagerSubsystem::PushConfirmationPopup(FText CategoryName, float Timeout)
{
    if (!UIConfig || !UIConfig->ConfirmationPopupClass)
    {
        UE_LOG(LogTemp, Error, TEXT("UIManager: ConfirmationPopupClass nav iestatīta UIConfig!"));
        return nullptr;
    }

    UUIConfirmationPopup* Popup = CreateWidget<UUIConfirmationPopup>(GetWorld(), UIConfig->ConfirmationPopupClass);
    if (Popup)
    {
        // Pievienojam stakam un parādām
        PushWidget(Popup, true, false);
        Popup->ShowPopup(CategoryName, Timeout);
    }

    return Popup;
}

void UUIManagerSubsystem::PushWidget(UUserWidget* NewWidget, bool bShowCursor, bool bPauseGame)
{
    if (!NewWidget) return;

    if (!NewWidget->IsInViewport())
    {
        // Pievienojam ar augstu ZOrder (piem. 10), lai tas nebūtu aiz melna Splash screen
        NewWidget->AddToViewport(10);
    }
    
    WidgetStack.Push(NewWidget);

    if (bPauseGame) 
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }

    UpdateInputMode();
}

void UUIManagerSubsystem::PopWidget()
{
    if (WidgetStack.Num() == 0) return;

    UUserWidget* TopWidget = WidgetStack.Pop();
    if (TopWidget)
    {
        TopWidget->RemoveFromParent();
    }

    // Ja staks ir tukšs, atpauzējam spēli
    if (WidgetStack.Num() == 0)
    {
        UGameplayStatics::SetGamePaused(GetWorld(), false);
    }

    UpdateInputMode();
}

// UIManagerSubsystem.cpp
void UUIManagerSubsystem::UpdateInputMode()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    if (WidgetStack.Num() > 0)
    {
        // IZMANTO ŠO, lai neizmet no spēles:
        FInputModeGameAndUI InputMode; 
        InputMode.SetWidgetToFocus(WidgetStack.Last()->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
    else
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}

void UUIManagerSubsystem::ClearAllWidgets()
{
    for (UUserWidget* Widget : WidgetStack)
    {
        if (Widget) Widget->RemoveFromParent();
    }
    WidgetStack.Empty();
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    UpdateInputMode();
}