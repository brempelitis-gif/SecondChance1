#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

class USaveGameData;

UCLASS()
class SECONDCHANCE_API USaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static constexpr int32 MaxSlots = 10;

	TArray<USaveGameData*> GetAllSaves() const;

	void CreateNewSave(const FString& PlayerName);
	void LoadSave(int32 SlotIndex);

	bool HasAnySaves() const;

private:
	FString GetSlotName(int32 Index) const;
};