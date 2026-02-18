#include "Core/Components/PlayerStatsComponent.h"
#include "Core/Enums/EPlayerStatType.h"
#include "TimerManager.h"
#include "Engine/World.h"

UPlayerStatsComponent::UPlayerStatsComponent()
{
    // PILNĪBĀ izslēdzam Tick šim komponentam
    PrimaryComponentTick.bCanEverTick = false;

    Stats.Add(EPlayerStatType::Hunger, 100.0f);
    Stats.Add(EPlayerStatType::Energy, 100.0f);
    Stats.Add(EPlayerStatType::Social, 100.0f);
    Stats.Add(EPlayerStatType::Hygiene, 100.0f);
    Stats.Add(EPlayerStatType::Bladder, 100.0f);
    Stats.Add(EPlayerStatType::Fun, 100.0f);
    Stats.Add(EPlayerStatType::Health, 100.0f);
}

void UPlayerStatsComponent::BeginPlay()
{
    Super::BeginPlay();

    // Palaižam taimeri. Tas automātiski apstāsies, kad spēle būs pauzēta.
    GetWorld()->GetTimerManager().SetTimer(
        StatsUpdateTimerHandle, 
        this, 
        &UPlayerStatsComponent::ProcessStatsUpdate, 
        UpdateFrequency, 
        true
    );
}

void UPlayerStatsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Drošībai notīrām taimeri, kad komponents tiek iznīcināts
    GetWorld()->GetTimerManager().ClearTimer(StatsUpdateTimerHandle);
    Super::EndPlay(EndPlayReason);
}

void UPlayerStatsComponent::ProcessStatsUpdate()
{
    for (auto& Pair : Stats)
    {
        // Health nekrītas pats no sevis
        if (Pair.Key == EPlayerStatType::Health) continue;

        float OldValue = Pair.Value;
        // Atņemam diltu, ņemot vērā cik bieži taimeris izpildās
        float NewValue = FMath::Clamp(OldValue - (GlobalDecayRate * UpdateFrequency), 0.0f, 100.0f);

        if (!FMath::IsNearlyEqual(OldValue, NewValue))
        {
            Pair.Value = NewValue;
            OnStatChanged.Broadcast(Pair.Key, NewValue);
        }
    }
}

float UPlayerStatsComponent::GetStatValue(EPlayerStatType StatType) const
{
    const float* Val = Stats.Find(StatType);
    return Val ? *Val : 0.0f;
}

void UPlayerStatsComponent::ModifyStat(EPlayerStatType StatType, float Amount)
{
    if (Stats.Contains(StatType))
    {
        float OldValue = Stats[StatType];
        float NewValue = FMath::Clamp(OldValue + Amount, 0.0f, 100.0f);
        
        Stats[StatType] = NewValue;
        OnStatChanged.Broadcast(StatType, NewValue);
    }
}

