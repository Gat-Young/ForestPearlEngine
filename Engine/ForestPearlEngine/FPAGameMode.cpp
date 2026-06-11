#include "FPAGameMode.h"
#include "FPWorld.h"

void FPAGameMode::Initialize()
{
	for (std::string ControllerName : ControllerList)
	{
		//게임에 사용할 컨트롤러 오브젝트를 만든다.
		GameController.push_back(GetWorld()->CreateClassInstnce<FPAController>(ControllerName));
		GameController.back()->SetOuter(this);
		GameController.back()->Initialize();
	}
}

void FPAGameMode::BeginPlay()
{
	for (FPAController* Controller : GameController)
	{
		Controller->BeginPlay();
	}
}

void FPAGameMode::Tick()
{
	for (FPAController* Controller : GameController)
	{
		Controller->Tick();
	}
}
