#pragma once

#include "GameFramework/SaveGame.h"
#include "AudioSettingsSaveGame.generated.h"

UCLASS()
class SECONDCHANCE_API UAudioSettingsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 0..1
	UPROPERTY(BlueprintReadWrite)
	float MasterVolume = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float MusicVolume = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float SFXVolume = 1.f;
};
