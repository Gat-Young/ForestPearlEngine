#include "GameLevel.h"
#include "Triangle.h"
#include "UI.h"

void GameLevel::Initialize()
{
	//게임에 사용할 게임 오브젝트를 만든다.
	GameObjectList.push_back(GetWorld()->CreateClassInstnce<Triangle>("Triangle"));
	GameObjectList.back()->SetOuter(this);
	GameObjectList.back()->Initialize();

	GameObjectList.push_back(GetWorld()->CreateClassInstnce<UI>("UI"));
	GameObjectList.back()->SetOuter(this);
	GameObjectList.back()->Initialize();
}

void GameLevel::BeginPlay()
{
	__super::BeginPlay();
}

void GameLevel::Tick()
{
	__super::Tick();
}