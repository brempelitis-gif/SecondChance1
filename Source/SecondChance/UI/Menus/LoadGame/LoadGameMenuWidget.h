#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadGameMenuWidget.generated.h"

class UScrollBox;
class UMenuButtonWidget;
class USaveSlotWidget;

UCLASS()
class SECONDCHANCE_API ULoadGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	void BindButtons();
	virtual void NativeOnInitialized() override;
	void NativePreConstruct();

	// --- UI Elementi ---
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SaveListScrollBox;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* BackBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText BackBtnLabel = FText::FromString("Back");

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* DeleteBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText DeleteBtnLabel = FText::FromString("Delete");
	
	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* LoadBtn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText LoadBtnLabel = FText::FromString("Load Game");

	// Šis ir svarīgi: Blueprintā šeit iepazīstināsim ar tavu SaveSlotWidget dizainu
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<USaveSlotWidget> SaveSlotWidgetClass;

private:
	// Funkcija, kas "piepilda" sarakstu
	void RefreshSaveList();

	// Funkcija, kas reaģē uz slota izvēli
	UFUNCTION()
	void HandleSlotSelected(FString SlotName);
	UFUNCTION()
	void HandleDeleteClicked();
	UFUNCTION()
	void HandleLoadClicked();

	UFUNCTION()
	void HandleBackClicked();

	// Glabāsim izvēlētā slota nosaukumu
	FString SelectedSlotName;
};