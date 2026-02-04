#pragma once

#include "CoreMinimal.h"
#include "ESettingsCategory.generated.h"

UENUM(BlueprintType)
enum class ESettingsCategory : uint8
{
	None,
	Audio,
	Graphics,
	Controls,
	Gameplay
};

// Definējam to šeit, lai tas būtu pieejams visur, kur ir pieejams enums
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsChanged, ESettingsCategory, Category);