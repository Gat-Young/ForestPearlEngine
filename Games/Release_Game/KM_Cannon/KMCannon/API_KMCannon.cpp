#include "API_KMCannon.h"
#include "Cannon.h"
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
