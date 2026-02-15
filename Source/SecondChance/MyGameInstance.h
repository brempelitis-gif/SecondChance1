#pragma once

#include "Engine/GameInstance.h"
#include "Core/Structs/FCharacterCustomizationData.h"
#include "MyGameInstance.generated.h"

class UUIManagerSubsystem;
class UUIConfig;


UCLASS(BlueprintType)
class SECONDCHANCE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SplashScreenClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	UUIConfig* UIConfig; // Piesaistām Editorā

	UFUNCTION(BlueprintCallable, Category="UI")
	UUIManagerSubsystem* GetUIManager() const;

	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
	FCharacterCustomizationData FinalCharacterData;

	// Funkcija asinhronai ielādei
	UFUNCTION(BlueprintCallable, Category = "Transitions")
	void AsyncLoadGameLevel(FName LevelName);


	// Konstante indeksa faila nosaukumam
	const FString MASTER_SAVE_INDEX = "MasterSaveIndex";

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	FString CreateNewSaveGame(FCharacterCustomizationData CharacterData);

	/** Nosaukums slotam, kuru mēs gribam ielādēt pēc līmeņa maiņas */
	UPROPERTY(BlueprintReadWrite, Category = "SaveSystem")
	FString CurrentSlotToLoad;

	/** Vai šī ir ielāde no seiva (true) vai jauna spēle (false)? */
	UPROPERTY(BlueprintReadWrite, Category = "SaveSystem")
	bool bIsLoadingFromSave = false;

private:
	void UpdateSaveIndex(FString SlotName, FString PlayerName);
protected:
	void OnLevelLoaded();
};