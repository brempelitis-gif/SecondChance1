#include "Player/SecondChancePlayerState.h"

ASecondChancePlayerState::ASecondChancePlayerState() { }

void ASecondChancePlayerState::AddMoney(int32 Amount)
{
	CurrentMoney = FMath::Max(0, CurrentMoney + Amount);
	OnMoneyChanged.Broadcast(CurrentMoney);
}
