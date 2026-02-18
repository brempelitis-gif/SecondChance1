#include "Core/Time/GameTimeSubsystem.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UGameTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Iestatām taimeri, kas atkārtojas (bLoop = true)
	// Taimeris Unreal dzinējā pēc noklusējuma REAĢĒ uz laika paātrināšanu/pauzi
	GetWorld()->GetTimerManager().SetTimer(
		TimeUpdateHandle, 
		this, 
		&UGameTimeSubsystem::AdvanceTime, 
		GameMinuteDuration, 
		true
	);
}

void UGameTimeSubsystem::Deinitialize()
{
	GetWorld()->GetTimerManager().ClearTimer(TimeUpdateHandle);
	Super::Deinitialize();
}

void UGameTimeSubsystem::AdvanceTime()
{
	CurrentMinutes++;

	if (CurrentMinutes >= 60)
	{
		CurrentMinutes = 0;
		CurrentHours++;
		if (CurrentHours >= 24) CurrentHours = 0;
	}

	OnTimeChanged.Broadcast(CurrentHours, CurrentMinutes);
}

FText UGameTimeSubsystem::GetFormattedTime() const
{
	return FText::FromString(FString::Printf(TEXT("%02d:%02d"), CurrentHours, CurrentMinutes));
}