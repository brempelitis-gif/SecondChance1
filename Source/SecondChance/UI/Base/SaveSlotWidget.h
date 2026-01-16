#pragma once

#include "UIBaseWidget.h"
#include "SaveSlotWidget.generated.h"

class USaveGameData;
class UTextBlock;
class UButton;

UCLASS()
class SECONDCHANCE_API USaveSlotWidget : public UUIBaseWidget
{
	GENERATED_BODY()

public:
	void Init(USaveGameData* InSaveData);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DateText;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;

private:
	UPROPERTY()
	USaveGameData* SaveData;
};
