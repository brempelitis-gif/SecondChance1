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
	
	void Initialize(FSubsystemCollectionBase& Collection);
	// Galvenā funkcija logrīka parādīšanai
	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void PushWidget(UUserWidget* NewWidget, bool bShowCursor = true, bool bPauseGame = false);

	// Noņem pēdējo logrīku un atjauno iepriekšējo stāvokli
	UFUNCTION(BlueprintCallable, Category = "UI Management")
	void PopWidget();

	// Palīgs, lai ātri aizvērtu visus logus (piem. ielādējot jaunu līmeni)
	void ClearAllWidgets();

private:
	// Steka (Stack) struktūra, lai mēs varētu atvērt logu virs loga (piem. Options virs Main Menu)
	UPROPERTY()
	TArray<UUserWidget*> WidgetStack;

	void UpdateInputMode();
};
