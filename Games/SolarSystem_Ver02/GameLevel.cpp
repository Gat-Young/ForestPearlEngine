#include "GameLevel.h"
#include "Triangle.h"
#include "UI.h"
#include "../../Engine/ForestPearlEngine/Utility/FPGameplayStatics.h"

void GameLevel::Initialize()
{
	ActorlList.push_back({ "GameCamera", "GameCamera" });
	ActorlList.push_back({ "Triangle", "Parent" });
	ActorlList.push_back({ "Triangle", "Child"});
	ActorlList.push_back({ "Triangle", "GrandChild"});
	ActorlList.push_back({ "UI", "UI"});
	ActorlList.push_back({ "Grid", "Grid"});
	__super::Initialize();
}

void GameLevel::BeginPlay()
{
	std::vector<FPActor*> Triangles;

	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "Triangle", Triangles);

	Triangle* Parent = nullptr;
	Triangle* Child = nullptr;
	Triangle* GrandChild = nullptr;

	for (FPActor* Actor : Triangles)
	{
		if (Actor->GetName() == "Parent")
		{
			Parent = dynamic_cast<Triangle*>(Actor);
		}

		if (Actor->GetName() == "Child")
		{
			Child = dynamic_cast<Triangle*>(Actor);
		}

		if (Actor->GetName() == "GrandChild")
		{
			GrandChild = dynamic_cast<Triangle*>(Actor);
		}
	}

	Child->SetActorLocation(FPVector3{ 1.0f, 0.0f, 0.0f });
	Child->SetActorScale3D(FPVector3{ 0.7f, 0.7f, 0.7f });

	GrandChild->SetActorLocation(FPVector3{ 0.0f, 0.0f, -1.0f });
	GrandChild->SetActorScale3D(FPVector3{ 0.5f, 0.5f, 0.5f });


	Child->AttachToActor(Parent);
	GrandChild->AttachToActor(Child);

	__super::BeginPlay();
}

void GameLevel::Tick()
{
	__super::Tick();
}