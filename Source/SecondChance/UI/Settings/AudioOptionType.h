#pragma once

#include "CoreMinimal.h"
#include "AudioOptionType.generated.h"

/**
 * Audio option types for UI sliders
 */
UENUM(BlueprintType)
enum class EAudioOption : uint8
{
	Master UMETA(DisplayName = "Master Volume"),
	Music  UMETA(DisplayName = "Music Volume"),
	SFX    UMETA(DisplayName = "SFX Volume")
};
