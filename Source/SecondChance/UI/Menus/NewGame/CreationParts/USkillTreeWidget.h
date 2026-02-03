#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "USkillTreeWidget.generated.h"

// Delegāts, lai pasacītu Master logrīkam iet atpakaļ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTreeNavigationRequested);

UCLASS()
class SECONDCHANCE_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Navigation")
	FOnSkillTreeNavigationRequested OnBackStepRequested;

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* BackBtn;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* PlayBtn;
	
	// --- Teksti (Rediģējami no Blueprint Details paneļa) ---
	UPROPERTY(EditAnywhere, Category = "Labels")
	FText BackButtonText = FText::FromString("Back");

	UPROPERTY(EditAnywhere, Category = "Labels")
	FText PlayButtonText = FText::FromString("Start Adventure");

	UFUNCTION() void HandleBackClicked();
	UFUNCTION() void HandlePlayClicked();
};
