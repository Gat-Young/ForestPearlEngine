#include "GameProjectClassRegistry.h"
#include "FPGameInstance.h"
#include "FPWorld.h"

void FPWorld::Initialize()
{
	if (!GameProjectClassRegistry::Get().HasFactory(LevelList[0])) { return; }

	PersistentLevel.reset(CreateClassInstnce<FPLevel>(LevelList[0]));
	PersistentLevel->SetOuter(this);
	PersistentLevel->Initialize();
}

void FPWorld::BeginPlay()
{
	PersistentLevel->BeginPlay();
}

void FPWorld::Tick()
{
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
	if (std::find(LevelList.begin(), LevelList.end(), LevelName) == LevelList.end()) { return; }
	FPLevel* Level = CreateClassInstnce<FPLevel>(LevelName);

	StreamingLevel[LevelName] = std::make_unique<FPLevel>(*Level);
	StreamingLevel[LevelName]->SetOuter(this);
	StreamingLevel[LevelName]->Initialize();

	StreamingLevel[LevelName]->BeginPlay();
}