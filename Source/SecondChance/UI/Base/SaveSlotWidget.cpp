#include "SaveSlotWidget.h"
#include "Core/Save/SaveGameData.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USaveSlotWidget::Init(USaveGameData* InSaveData)
{
	SaveData = InSaveData;

	PlayerNameText->SetText(FText::FromString(SaveData->PlayerName));
	DateText->SetText(FText::FromString(SaveData->SaveDate.ToString()));
}