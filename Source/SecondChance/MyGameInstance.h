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

protected:
	void OnLevelLoaded();
};