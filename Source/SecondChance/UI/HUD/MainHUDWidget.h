#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/Enums/EPlayerStatType.h" // Pārliecinies, ka enums ir pieejams
#include "MainHUDWidget.generated.h"

class UPlayerStatsComponent;
class UStatBarWidget;

UCLASS()
class SECONDCHANCE_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	/** Funkcija, kas saņems ziņas no komponenta */
	UFUNCTION()
	void HandleStatChanged(EPlayerStatType StatType, float NewValue);

	/** Palīgfunkcija, lai atrastu pareizo joslu pēc tipa */
	UStatBarWidget* GetStatBarByType(EPlayerStatType StatType);

	// Šeit mēs sasaistām konkrētus WBP bērnus ar C++
	// WBP pusē tev būs jāieliek šie StatBarWidget un jānofiksē vārdi
	UPROPERTY(meta = (BindWidget))
	UStatBarWidget* StatBar_Hunger;

	UPROPERTY(meta = (BindWidget))
	UStatBarWidget* StatBar_Health;

	UPROPERTY(meta = (BindWidget))
	UStatBarWidget* StatBar_Energy;

private:
	UPROPERTY()
	UPlayerStatsComponent* CachedStatsComp;
};