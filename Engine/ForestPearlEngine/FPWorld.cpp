#include "GameProjectClassRegistry.h"
#include "FPGameInstance.h"
#include "FPWorld.h"

void FPWorld::Initialize()
{

	if (!GameProjectClassRegistry::Get().HasFactory(WorldSetting.GameMode)) { return; }
	GameMode.reset(CreateClassInstnce<FPAGameMode>(WorldSetting.GameMode));
	GameMode->SetOuter(this);
	GameMode->Initialize();

	if (!GameProjectClassRegistry::Get().HasFactory(WorldSetting.LevelList[0])) { return; }

	PersistentLevel.reset(CreateClassInstnce<FPLevel>(WorldSetting.LevelList[0]));
	PersistentLevel->SetOuter(this);
	PersistentLevel->Initialize();

}

void FPWorld::BeginPlay()
{
	GameMode->BeginPlay();
	PersistentLevel->BeginPlay();
}

void FPWorld::Tick()
{
	GameMode->Tick();

	PersistentLevel->Tick();

	for (auto& Level : StreamingLevel)
	{
		Level.second->Tick();
	}
}

void FPWorld::UnLoadData(std::string LevelName)
{
	auto it = StreamingLevel.find(LevelName);

	if (it == StreamingLevel.end())
	{
		return;
	}

	it->second->UnLoadData();
	it->second.release();

	StreamingLevel.erase(LevelName);
}

void FPWorld::Finalize()
{
	for (auto& Levels : StreamingLevel)
	{
		UnLoadData(Levels.first);
	}

	PersistentLevel->UnLoadData();
	PersistentLevel.release();
}


void FPWorld::OpenLevel(std::string LevelName)
{
	if (std::find(WorldSetting.LevelList.begin(), WorldSetting.LevelList.end(), LevelName) == WorldSetting.LevelList.end()) { return; }
	FPLevel* Level = CreateClassInstnce<FPLevel>(LevelName);

	StreamingLevel[LevelName] = std::make_unique<FPLevel>(*Level);
	StreamingLevel[LevelName]->SetOuter(this);
	StreamingLevel[LevelName]->Initialize();

	StreamingLevel[LevelName]->BeginPlay();
}

GameTimer* FPWorld::GetGameTimer()
{
	return FPGameInstance::Get().GetGameTimer();
}


FPActor* FPWorld::SpawnActor(std::string ActorClassName)
{
	FPActor* SpawnActor = CreateClassInstnce<FPActor>(ActorClassName);
	SpawnActor->SetOuter(PersistentLevel.get());
	SpawnActor->Initialize();
	SpawnActor->BeginPlay();

	GL->GetGameActorList().push_back(SpawnActor);
	int CurrentSpawnActorIndex = GL->GetCurrentActorCount();
	GL->GetCurrentActorCount()++;

	return CurrentSpawnActorIndex;
}