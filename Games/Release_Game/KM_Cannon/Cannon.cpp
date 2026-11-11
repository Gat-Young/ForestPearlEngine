#include "Cannon.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"

#include "ForestPearlEngine/Utility/FPGameplayStatics.h"

void Cannon::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Canon_1_StaticMesh");
	//Mesh->SetupAttachment(RootComponent); //루트에 자식
	SetRootComponent(Mesh); //내가 루트
}

void Cannon::BeginPlay()
{
	FPActor* ShipActor = FPGameplayStatics::GetActorOfClass(GetWorld(), "Ship");

	if (ShipActor == nullptr)
	{
		return;
	}

	// 대포의 루트를 배의 루트 아래에 부착
	AttachToActor(ShipActor);

	// 배를 기준으로 한 상대 위치·회전·크기
	Mesh->SetRelativeLocation({ 2.8f, 3.45f, -8.0f });
	Mesh->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
	Mesh->SetRelativeScale3D({ 0.33f, 0.33f, 0.33f });
}