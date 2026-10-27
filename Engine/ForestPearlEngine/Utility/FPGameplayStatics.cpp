#include "FPGameplayStatics.h"
#include "../Object/Actor.h"
#include "../FPWorld.h"
#include "../FPGameInstance.h"
#include "../FPGameProjectClassRegistry.h"
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>


FPActor* FPGameplayStatics::GetActorOfClass(FPWorld* World, std::string ClassName)
{
	std::vector<FPActor*> Actors = World->GetGameActorList();

	FPGameProjectClassRegistry* ClassRegister = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());

	const std::type_index* TargetType = ClassRegister->GetClassType(ClassName);

	if (TargetType == nullptr)
	{
		return nullptr;
	}

	for (FPActor* LevelActor : Actors)
	{
		if (LevelActor == nullptr)
			continue;

		if (std::type_index(typeid(*LevelActor)) == *TargetType)
		{
			return LevelActor;
		}
	}


	return nullptr;
}

void FPGameplayStatics::GetAllActorsOfClass(FPWorld* World, std::string ClassName, std::vector<FPActor*>& OutActors)
{

	std::vector<FPActor*> Acotrs = World->GetGameActorList();

	FPGameProjectClassRegistry* ClassRegister = static_cast<FPGameProjectClassRegistry*>(FPGameInstance::Get().GetClassRegister());

	const std::type_index* TargetType = ClassRegister->GetClassType(ClassName);

	if (TargetType == nullptr)
	{
		return;
	}


	for (FPActor* LevelActor : Acotrs)
	{
		if (LevelActor == nullptr)
			continue;

		if (std::type_index(typeid(*LevelActor)) == *TargetType)
		{
			OutActors.push_back(LevelActor);
		}
	}
}

std::vector<class FPActor*> FPGameplayStatics::GetAllActors(FPWorld* World)
{
	std::vector<FPActor*> Acotrs = World->GetGameActorList();
	return Acotrs;
}
