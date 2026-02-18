#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PortraitPanel.generated.h"

class UImage;

UCLASS()
class SECONDCHANCE_API UPortraitPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_PlayerPortrait;

	virtual void NativeConstruct() override;

	// Funkcija, kas ielādēs bildi
	UTexture2D* LoadTextureFromPath(const FString& Path);
};