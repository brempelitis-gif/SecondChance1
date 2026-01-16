#pragma once

#include "UIActions.generated.h"

UENUM(BlueprintType)
enum class EUIAction : uint8
{
	Resume,
	NewGame,
	Load,
	Options,
	Quit
};
