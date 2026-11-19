#include "CannonBall.h"
#include "ForestPearlEngine/FPSceneComponent.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPWorld.h"
#include <iostream>

//보면서 조절할 값들
namespace
{
	const float SPEED = 20.0f;		//포구 속도 (m/s)
	const float GRAVITY = 9.8f;		//중력 (m/s²)
	const float RADIUS = 0.15f;		//포탄 반지름 (m) : Sphere_StaticMesh 크기 1 기준으로 스케일
	const float GROUND_Y = -0.5f;		//땅 높이 (레벨의 Terrain y) : 이 아래로 내려가면 착탄
	const float LIFE = 10.0f;		//땅에 안 닿아도 이 시간 지나면 회수 (초)
	const FPVector4 COLOR(0.08f, 0.08f, 0.08f, 1.0f);	//거무스름한 쇠
}

void CannonBall::Initialize()
{
	Root = new FPSceneComponent(this);
	SetRootComponent(Root);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	for (Ball& b : Balls)
	{
		b.Mesh = new FPStaticMeshComponent(this, "Sphere_StaticMesh");
		b.Mesh->SetupAttachment(Root);
		b.Mesh->SetRelativeScale3D({ RADIUS, RADIUS, RADIUS });

		//엔진 DefaultMaterial 을 그대로 고치면 다른 물체도 같이 검어짐 → 포탄마다 새로 만들어 씀
		FPMaterialInterface* Material = AssetManager->MakeMaterial("DefaultMaterial");
		if (Material != nullptr)
		{
			Material->SetParameter(FPShaderType::PS, "MaterialConstBuffer", "Material.BaseColor", COLOR);
			Material->SetParameter(FPShaderType::PS, "MaterialConstBuffer", "Material.Diffuse", COLOR);
			b.Mesh->SetMaterial(Material);
		}
		else std::cout << "[CannonBall] MakeMaterial failed : DefaultMaterial\n";

		b.Mesh->SetActive(false);
	}
}

void CannonBall::BeginPlay()
{
	//레벨 JSON 의 위치와 상관없이 월드 원점에 둠 → 자식 상대 위치 = 월드 위치
	SetActorLocation({ 0.0f, 0.0f, 0.0f });
	SetActorRotation({ 0.0f, 0.0f, 0.0f });
	SetActorScale3D({ 1.0f, 1.0f, 1.0f });
}

void CannonBall::Launch(const FPVector3& Pos, const FPVector3& Dir)
{
	for (Ball& b : Balls)
	{
		if (b.bLive) continue;
		b.Pos = Pos + Dir * RADIUS;		//포구 안쪽에서 생기지 않게 반지름만큼 앞
		b.Vel = Dir * SPEED;
		b.Age = 0;
		b.bLive = true;
		b.Mesh->SetRelativeLocation(b.Pos);
		b.Mesh->SetActive(true);
		return;
	}
}

void CannonBall::Tick()
{
	float dTime = GetWorld()->GetGameTimer()->DeltaTime();

	for (Ball& b : Balls)
	{
		if (!b.bLive) continue;

		b.Age += dTime;
		b.Vel.y -= GRAVITY * dTime;
		b.Pos = b.Pos + b.Vel * dTime;

		if (b.Pos.y - RADIUS <= GROUND_Y || b.Age > LIFE)	//착탄 (착탄 이펙트는 여기서)
		{
			b.bLive = false;
			b.Mesh->SetActive(false);
			continue;
		}
		b.Mesh->SetRelativeLocation(b.Pos);
	}

	__super::Tick();		//위치를 바꾼 뒤 : 루트 → 자식 행렬 계산
}