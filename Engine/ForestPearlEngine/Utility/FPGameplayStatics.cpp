#include "FPGameplayStatics.h"
#include "../Object/Actor.h"
#include "../FPWorld.h"
#include "../GameProjectClassRegistry.h"
#include <typeinfo>
#include <utility>
#include <vector>


FPActor* FPGameplayStatics::GetActorOfClass(FPWorld* World, std::string ClassName)
{
	std::vector<FPActor*> Acotrs = World->GetGameActorList();

	std::unique_ptr<FPObject> Object = GameProjectClassRegistry::Get().Create(ClassName);
	FPActor* TargetActor = dynamic_cast<FPActor*>(Object.get());

	for (FPActor* LevelActor : Acotrs)
	{
		if (typeid(LevelActor) == typeid(TargetActor))
		{
			return LevelActor;
		}
	}


	return nullptr;
}

void FPGameplayStatics::GetAllActorsOfClass(FPWorld* World, std::string ClassName, std::vector<FPActor*>& OutActors)
{

	std::vector<FPActor*> Acotrs = World->GetGameActorList();

	std::unique_ptr<FPObject> Object = GameProjectClassRegistry::Get().Create(ClassName);
	FPActor* TargetActor = dynamic_cast<FPActor*>(Object.get());

	for (FPActor* LevelActor : Acotrs)
	{
		if (typeid(LevelActor) == typeid(TargetActor))
		{
			OutActors.push_back(LevelActor);
		}
	}
}
