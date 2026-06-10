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

		GameWorld.World.reset(World);
	}
}

FPWorld* FPGameInstance::GetWorld()
{
	return GameWorld.World.get();
}

void FPGameInstance::Initialize()
{
	GameWorld.World->Initialize();
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
	GameWorld.World->UnLoadData(GameWorld.WorldName);
}

void FPGameInstance::Finalize()
{
	GameWorld.World->Finalize();
}
