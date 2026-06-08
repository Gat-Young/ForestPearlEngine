#include "FPGameInstance.h"

void FPGameInstance::OpenLevel(std::string WorldName)
{
	if (GameWorld.World != nullptr) GameWorld.World.reset();

	GameWorld.WorldName = WorldName;

	if (GameProjectClassRegistry::Get().HasFactory(WorldName))
	{
		std::unique_ptr<FPObject> WorldObject = GameProjectClassRegistry::Get().Create(WorldName);

		FPWorld* World = dynamic_cast<FPWorld*>(WorldObject.get());

		WorldObject.release();

		GameWorld.World = std::make_unique<FPWorld>(World);
	}
}

FPWorld* FPGameInstance::GetWorld()
{
	return GameWorld.World.get();
}

void FPGameInstance::LoadData()
{
	GameWorld.World->LoadData();
}

void FPGameInstance::BeginPlay()
{
	GameWorld.World->BeginPlay();
}

void FPGameInstance::Tick()
{
	GameWorld.World->Tick();
}

void FPGameInstance::UnLoadData()
{
	GameWorld.World->UnLoadData();
}

void FPGameInstance::Finalize()
{
	GameWorld.World->Finalize();
}
