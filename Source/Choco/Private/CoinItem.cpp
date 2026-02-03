#include "CoinItem.h"
#include "Engine/World.h"
#include "HunGameState.h"



ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (AHunGameState* GameState = World->GetGameState<AHunGameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}


		DestroyItem();
	}
}