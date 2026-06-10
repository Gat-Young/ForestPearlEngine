#include "GameProjectClassRegistry.h"
#include "FPGameInstance.h"
#include "FPLevel.h"

void FPLevel::BeginPlay()
{
	for (FPObject* obj : GameObjectList)
	{
		obj->BeginPlay();
	}
}

void FPLevel::Tick()
{
	for (FPObject* obj : GameObjectList)
	{
		obj->Tick();
	}
}

void FPLevel::UnLoadData()
{
	for (FPObject* obj : GameObjectList)
	{
		delete(obj);
	}
}


void FPLevel::Finalize()
{
	UnLoadData();
}