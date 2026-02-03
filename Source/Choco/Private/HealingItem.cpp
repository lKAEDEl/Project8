#include "HealingItem.h"
#include "HunCharacter.h"




AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (AHunCharacter* PlayerCharacter = Cast<AHunCharacter>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}


		DestroyItem();
	}
}
