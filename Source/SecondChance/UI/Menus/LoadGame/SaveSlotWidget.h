#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Structs/FSaveMetadata.h" // Tava jaunā metadatu struktūra
#include "SaveSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UMenuButtonWidget;

// Delegāts, lai galvenā izvēlne zinātu, kurš slots ir izvēlēts
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveSlotSelected, FString, SlotName);

UCLASS()
class SECONDCHANCE_API USaveSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Funkcija, ko izsauksim no saraksta, lai aizpildītu datus
	void SetupSlot(const FSaveMetadata& Metadata);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSaveSlotSelected OnSlotSelected;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	

	// --- UI Elementi ---
	UPROPERTY(meta = (BindWidget))
	UImage* PortraitImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SaveDateText;

	UPROPERTY(meta = (BindWidget))
	UMenuButtonWidget* SelectBtn; 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Main Menu|Labels")
	FText SelectBtnLabel = FText::FromString("Select");

private:
	UFUNCTION()
	void HandleBtnClicked();
	
	void BindButtons();
	
	FString CachedSlotName;

	// Palīgfunkcija, lai ielādētu .png bildi no diska
	UTexture2D* LoadTextureFromFile(const FString& FullPath);
};
