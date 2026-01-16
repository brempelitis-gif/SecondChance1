#include "SaveGameSubsystem.h"
#include "SaveGameData.h"
#include "Kismet/GameplayStatics.h"

TArray<USaveGameData*> USaveGameSubsystem::GetAllSaves() const
{
	TArray<USaveGameData*> Result;

	for (int32 i = 0; i < MaxSlots; ++i)
	{
		FString Slot = GetSlotName(i);
		if (UGameplayStatics::DoesSaveGameExist(Slot, 0))
		{
			if (USaveGameData* Save =
				Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(Slot, 0)))
			{
				Result.Add(Save);
			}
		}
	}

	return Result;
}

void USaveGameSubsystem::CreateNewSave(const FString& PlayerName)
{
	// Ja pilns – dzēš vecāko
	TArray<USaveGameData*> Saves = GetAllSaves();
	if (Saves.Num() >= MaxSlots)
	{
		Saves.Sort([](const USaveGameData& A, const USaveGameData& B)
		{
			return A.SaveDate < B.SaveDate;
		});

		UGameplayStatics::DeleteGameInSlot(
			GetSlotName(Saves[0]->SlotIndex), 0);
	}

	int32 NewSlot = FMath::RandRange(0, MaxSlots - 1);

	USaveGameData* NewSave =
		Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));

	NewSave->PlayerName = PlayerName;
	NewSave->SaveDate = FDateTime::Now();
	NewSave->SlotIndex = NewSlot;

	UGameplayStatics::SaveGameToSlot(NewSave, GetSlotName(NewSlot), 0);
}

bool USaveGameSubsystem::HasAnySaves() const
{
	for (int32 i = 0; i < MaxSlots; ++i)
	{
		if (UGameplayStatics::DoesSaveGameExist(GetSlotName(i), 0))
			return true;
	}
	return false;
}

FString USaveGameSubsystem::GetSlotName(int32 Index) const
{
	return FString::Printf(TEXT("SaveSlot_%d"), Index);
}