
#include "HunGameInstance.h"

UHunGameInstance::UHunGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UHunGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}