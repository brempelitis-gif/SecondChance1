
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Structs/FCharacterCustomizationData.h"
#include "UI/Base/MenuButton/MenuButtonWidget.h"
#include "UCharacterSkillTreeWidget.generated.h"

// Delegāts, lai pasacītu Master logrīkam iet atpakaļ
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillTreeNavigationRequested);

UCLASS()
class SECONDCHANCE_API UCharacterSkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Navigation")
	FOnSkillTreeNavigationRequested OnBackStepRequested;
	
	// Funkcija, lai saņemtu datus no Master logrīka
    void InitSkillTree(FCharacterCustomizationData& Data) { CurrentData = Data; UpdateUI(); }

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	
	FCharacterCustomizationData CurrentData;
    void UpdateUI(); // Atjauno teksta laukus ekrānā
	
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