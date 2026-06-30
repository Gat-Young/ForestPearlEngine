#include "GameWorld.h"
#include "../../Engine/ForestPearlEngine/ForestPearlEngine.h"
#include "UI.h"
#include "Orb.h"
#include "GameLevel.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include <iostream>

void GameWorld::Initialize()
{
	//World에서 사용되는 GameMode class 등록
	WorldSetting.GameMode = "GameMode";
	//World에서 사용되는 레벨 class 등록
	WorldSetting.LevelList.push_back("GameLevel");
	__super::Initialize();
}

void GameWorld::BeginPlay()
{

	__super::BeginPlay();

}

void GameWorld::Tick()
{
	///게임 실행 부분
	__super::Tick();
}

int GameWorld::SpawnActor(std::string ActorClassName)
{
	GameLevel* GL = dynamic_cast<GameLevel*>(PersistentLevel.get());

	if (GL == nullptr) return -1;

	FPActor* SpawnActor = CreateClassInstnce<FPActor>(ActorClassName);
	SpawnActor->SetOuter(this);
	SpawnActor->Initialize();
	SpawnActor->BeginPlay();

	GL->GetGameActorList().push_back(SpawnActor);
	int CurrentSpawnActorIndex = GL->GetCurrentActorCount();
	GL->GetCurrentActorCount()++;

	return CurrentSpawnActorIndex;
}

std::vector<FPActor*>& GameWorld::GetGameActorList()
{
	GameLevel* GL = dynamic_cast<GameLevel*>(PersistentLevel.get());

	if (GL == nullptr)
	{
		std::cout << "No GameLevel" << "\n";
		static std::vector<FPActor*> Empty;
		return Empty;
	}

	return GL->GetGameActorList();
}

const int GameWorld::GetCurrentActorCount()
{
	GameLevel* GL = dynamic_cast<GameLevel*>(PersistentLevel.get());

	if (GL == nullptr)
	{
		std::cout << "No GameLevel" << "\n";
		return -1;
	}

	return GL->GetCurrentActorCount();
}
