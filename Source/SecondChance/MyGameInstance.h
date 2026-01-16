#pragma once

#include "Engine/GameInstance.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	UUIConfig* UIConfig; // Piesaistām Editorā

	UFUNCTION(BlueprintCallable, Category="UI")
	UUIManagerSubsystem* GetUIManager() const;
};