#include "FPWorld.h"
#include "FPAssetManager.h"
#include "FPAGameMode.h"
#include "Object/Actor.h"
#include "FPGameTimer.h"
#include <iostream>

FPWorld::FPWorld() = default;

FPWorld::~FPWorld() = default;

void FPWorld::Initialize()
{
	GameMode->Initialize();
	for (FPActor* actor : GameActorList)
	{
		actor->Initialize();
	}
}

void FPWorld::BeginPlay()
{
	GameMode->BeginPlay();
	for (FPActor* actor : GameActorList)
	{
		actor->BeginPlay();
	}
}

void FPWorld::Tick()
{
	GameMode->Tick();

	for (FPActor* actor : GameActorList)
	{
		actor->Tick();
	}
}

void FPWorld::UnLoadData()
{
	for (FPActor* actor : GameActorList)
	{
		delete(actor);
	}
}

void FPWorld::Finalize()
{
	UnLoadData();
}


//json Level Data를 읽어와 Actor List를 초기화
//GameMode를 생성
void FPWorld::OpenLevel(std::string LevelName)
{
	FPGameInstance* GameInstance = static_cast<FPGameInstance*>(GetOuter());

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(GameInstance->GetAssetManager());

	FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(GameInstance->GetClassRegister());

	//GameMode 생성
	std::string GameModeName = AssetManager->GetGameModeData(LevelName);
	if (!ClassRegistry->HasFactory(GameModeName)) { std::cout << GameModeName << "의 Class가 존재하지 않음" << "\n"; return; }
	FPAGameMode* CreateGameMode = CreateClassInstnce<FPAGameMode>(GameModeName);
	CreateGameMode->SetOuter(this);
	GameMode.reset(CreateGameMode);

	//Level Data를 바탕으로 ActorList 초기화
	std::vector<FPActorData>& ActorData = AssetManager->GetLevelData(LevelName);

	for (FPActorData& Data : ActorData)
	{
		FPActor* SpawnActor = CreateClassInstnce<FPActor>(Data.ClassName);
		SpawnActor->SetOuter(this);
		SpawnActor->SetActorName(Data.ActorName);
		SpawnActor->SetActorLocation({ Data.Location_x,Data.Location_y, Data.Location_z });
		SpawnActor->SetActorRotation({ Data.Rotation_x, Data.Rotation_y, Data.Rotation_z });
		SpawnActor->SetActorScale3D({ Data.Scale_x, Data.Scale_y, Data.Scale_Z });

		GameActorList.push_back(SpawnActor);
	}
	

}

FPGameTimer* FPWorld::GetGameTimer()
{
	FPGameInstance* GameInstance = static_cast<FPGameInstance*>(GetOuter());
	
	return static_cast<FPGameTimer*>(GameInstance->GetGameTimer());
}


FPActor* FPWorld::SpawnActor(std::string ActorClassName, std::string ActorName)
{
	FPActor* SpawnActor = CreateClassInstnce<FPActor>(ActorClassName);
	SpawnActor->SetOuter(this);
	SpawnActor->Initialize();
	SpawnActor->BeginPlay();
	SpawnActor->SetActorName(ActorName);

	GameActorList.push_back(SpawnActor);

	return SpawnActor;
}

FPAController* FPWorld::GetController(int index)
{
	return GameMode->GetController(index);
}

FPAGameMode* FPWorld::GetAuthGameMode()
{
	return GameMode.get();
}

std::vector<FPActor*>& FPWorld::GetGameActorList()
{
	return GameActorList;
}