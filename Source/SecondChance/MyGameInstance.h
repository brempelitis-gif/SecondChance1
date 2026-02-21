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
	virtual void LoadComplete(float LoadTime, const FString& MapName) override;
	virtual void Shutdown() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	UUIConfig* UIConfig; // Piesaistām Editorā

	UPROPERTY()
	UUIManagerSubsystem* UIMan;
	
	UFUNCTION(BlueprintCallable, Category="UI")
	UUIManagerSubsystem* GetUIManager() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void TogglePauseMenu();
	
	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	void PrepareForLoad(FString SlotName);
	
	UPROPERTY(BlueprintReadWrite, Category = "Character Data")
	FCharacterCustomizationData FinalCharacterData;

	// Funkcija asinhronai ielādei
	UFUNCTION(BlueprintCallable, Category = "Transitions")
	void AsyncLoadGameLevel(FName LevelName);

	/** Saglabā pēdējā uzņemtā portreta nosaukumu */
	UPROPERTY(BlueprintReadWrite, Category = "SaveSystem")
	FString LastCapturedPortraitName;
	
	// Konstante indeksa faila nosaukumam
	const FString MASTER_SAVE_INDEX = "MasterSaveIndex";

	UFUNCTION(BlueprintCallable, Category = "SaveSystem")
	FString CreateNewSaveGame(FCharacterCustomizationData CharacterData);
	void ClearLoadData();

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