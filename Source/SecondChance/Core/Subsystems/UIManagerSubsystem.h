#pragma once

#include "CoreMinimal.h"
#include "UIConfig.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

UCLASS()
class SECONDCHANCE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI", meta=(AllowPrivateAccess="true"))
	UUIConfig* UIConfig;

	// Atgriež izveidoto logrīku, lai calleris (Options menu) varētu piesaistīt delegātus
	UUIConfirmationPopup* PushConfirmationPopup(FText CategoryName, float Timeout);
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void PushWidget(UUserWidget* NewWidget, bool bShowCursor = true, bool bPauseGame = false);

	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void PopWidget();

	void ClearAllWidgets();

private:
	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;

	void UpdateInputMode();
};