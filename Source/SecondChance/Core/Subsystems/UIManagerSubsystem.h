#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

class UUIConfig;
class UUserWidget;
class UUIConfirmationPopup;

UCLASS()
class SECONDCHANCE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// UI Konfigurācija (Satur klases/stilus)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	UUIConfig* UIConfig;

	// --- Logrīku pārvaldība ---
    
	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void PushWidget(UUserWidget* NewWidget, bool bShowCursor = true, bool bPauseGame = false);

	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void PopWidget();

	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void ClearAllWidgets();

	// Specifisks palīgs grafikas apstiprinājumiem u.c.
	UUIConfirmationPopup* PushConfirmationPopup(FText CategoryName, float Timeout);

	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;
	
	void UpdateInputMode();
};
