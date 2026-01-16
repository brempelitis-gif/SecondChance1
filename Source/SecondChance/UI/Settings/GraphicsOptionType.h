#pragma once
#include "CoreMinimal.h"
#include "GraphicsOptionType.generated.h"

/**
 * Graphics option types for UI Dropdons
 */
UENUM(BlueprintType)
enum class EGraphicsOption : uint8
{
	Resolution UMETA(DisplayName = "Resolution"),
	WindowMode UMETA(DisplayName = "Window mode"),
	Quality UMETA(DisplayName = "Quality")
};
