#include "FPGameInstance.h"

void FPGameInstance::OpenLevel(std::string WorldName)
{
	if (GameWorld.World != nullptr) GameWorld.World.reset();

	GameWorld.WorldName = WorldName;
	if (GameProjectClassRegistry::Get().HasFactory(WorldName))
	{
		GameWorld.World = GameProjectClassRegistry::Get().Create(WorldName);
	}
}

FPObject* FPGameInstance::GetWorld()
{
	return GameWorld.World.get();
}

void FPGameInstance::LoadData()
{

}

void FPGameInstance::BeginPlay()
{

}

void FPGameInstance::Tick()
{

}

void FPGameInstance::UnLoadData()
{

}

void FPGameInstance::Finalize()
{

}
