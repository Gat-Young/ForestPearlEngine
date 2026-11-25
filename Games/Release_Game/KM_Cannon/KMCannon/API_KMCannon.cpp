#include "API_KMCannon.h"
#include "Cannon.h"
#include "Frigate.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"

#include <vector>

int API_KMCannon::FireAll(FPWorld* World)
{
	std::vector<FPActor*> Cannons;
	FPGameplayStatics::GetAllActorsOfClass(World, "Cannon", Cannons);
	for (FPActor* Actor : Cannons)
	{
		static_cast<Cannon*>(Actor)->Fire();
	}
	return static_cast<int>(Cannons.size());
}

int API_KMCannon::FireBroadside(FPWorld* World, ESide Side, bool bLight)
{
	int Count = 0;
	std::vector<FPActor*> Ships;
	FPGameplayStatics::GetAllActorsOfClass(World, "Frigate", Ships);
	for (FPActor* Actor : Ships)
	{
		Count += static_cast<Frigate*>(Actor)->Fire(Side, bLight);
	}
	return Count;
}
