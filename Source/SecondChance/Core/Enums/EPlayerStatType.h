#pragma once

#include "CoreMinimal.h"
#include "EPlayerStatType.generated.h"

/** Universālie personāža stāvokļi */
UENUM(BlueprintType)
enum class EPlayerStatType : uint8
{
	Hunger,
	Energy,
	Social,
	Hygiene,
	Bladder,
	Fun,
	Health    // Pievienoju Health kā kritiski svarīgu funkciju
};