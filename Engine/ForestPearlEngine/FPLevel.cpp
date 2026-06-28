#include "GameProjectClassRegistry.h"
#include "FPGameInstance.h"
#include "FPLevel.h"

void FPLevel::Initialize()
{
	for (std::string ActorName : ActorlList)
	{
		//게임에 사용할 엑터를 만든다.
		GameActorList.push_back(GetWorld()->CreateClassInstnce<FPActor>(ActorName));
		GameActorList.back()->SetOuter(this);
		GameActorList.back()->Initialize();
	}

}

void FPLevel::BeginPlay()
{
	for (FPActor* actor : GameActorList)
	{
		actor->BeginPlay();
	}
}

void FPLevel::Tick()
{
	for (FPActor* actor : GameActorList)
	{
		actor->Tick();
	}
}

void FPLevel::UnLoadData()
{
	for (FPActor* actor : GameActorList)
	{
		delete(actor);
	}
}


void FPLevel::Finalize()
{
	UnLoadData();
}

void FPLevel::TryAddActorToList(FPActor* Actor)
{
	GameActorList.push_back(Actor);
}
