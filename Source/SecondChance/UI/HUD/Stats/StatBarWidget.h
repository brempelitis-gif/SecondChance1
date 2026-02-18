#pragma once

#include "CoreMinimal.h"
#include "UI/Base/UIBaseWidget.h" // Pieņemot, ka tev ir bāzes klase, ja nē - izmanto UUserWidget
#include "Core/Components/PlayerStatsComponent.h"
#include "Core/Enums/EPlayerStatType.h"
#include "StatBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SECONDCHANCE_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Iestata, kuru statu šī josla novēros */
	void InitializeStatBar(EPlayerStatType NewStatType);

	/** Atjauno vizuālo izskatu */
	void UpdateVisuals(float CurrentValue);

protected:
	/** Tips, ko šī josla attēlo (iestatāms arī WBP) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	EPlayerStatType StatToDisplay = EPlayerStatType::Hunger;

	// --- BindWidgets ---
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_Value;

	// Ja vēlies, lai teksts nebūtu obligāts, izmantojam BindWidgetOptional
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* Text_StatName;
	
	/** Teksta formāts, piemēram "Hunger: 85%" vai "85/100" */
	UPROPERTY(EditAnywhere, Category = "Stats")
	FText StatLabelFormat = FText::FromString("{0}");

	virtual void NativeConstruct() override;
};