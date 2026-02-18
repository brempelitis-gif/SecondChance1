#include "UI/HUD/MainHUDWidget.h"
#include "UI/HUD/Stats/StatBarWidget.h"
#include "Core/Components/PlayerStatsComponent.h"
#include "Kismet/GameplayStatics.h"

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 1. Atrodam spēlētāju un tā StatsComponent
	APawn* PlayerPawn = GetOwningPlayerPawn();
	if (PlayerPawn)
	{
		CachedStatsComp = PlayerPawn->FindComponentByClass<UPlayerStatsComponent>();
        
		if (CachedStatsComp)
		{
			// 2. Pierakstāmies uz izmaiņām
			CachedStatsComp->OnStatChanged.AddDynamic(this, &UMainHUDWidget::HandleStatChanged);

			// 3. Inicializējam sākuma vērtības
			// (Varētu iziet cauri visiem enumiem, bet šeit tieši uzrunājam svarīgākos)
			if(StatBar_Hunger) StatBar_Hunger->UpdateVisuals(CachedStatsComp->GetStatValue(EPlayerStatType::Hunger));
			if(StatBar_Health) StatBar_Health->UpdateVisuals(CachedStatsComp->GetStatValue(EPlayerStatType::Health));
			if(StatBar_Energy) StatBar_Energy->UpdateVisuals(CachedStatsComp->GetStatValue(EPlayerStatType::Energy));
		}
	}
}

void UMainHUDWidget::HandleStatChanged(EPlayerStatType StatType, float NewValue)
{
	// Atrodam pareizo joslu un liekam tai atjaunoties
	UStatBarWidget* TargetBar = GetStatBarByType(StatType);
	if (TargetBar)
	{
		TargetBar->UpdateVisuals(NewValue);
	}
}

UStatBarWidget* UMainHUDWidget::GetStatBarByType(EPlayerStatType StatType)
{
	switch (StatType)
	{
	case EPlayerStatType::Hunger: return StatBar_Hunger;
	case EPlayerStatType::Health: return StatBar_Health;
	case EPlayerStatType::Energy: return StatBar_Energy;
		// Pievieno pārējos pēc vajadzības
	default: return nullptr;
	}
}