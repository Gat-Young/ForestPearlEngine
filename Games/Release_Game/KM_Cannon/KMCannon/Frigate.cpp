#include "Frigate.h"
#include "ForestPearlEngine/FPSceneComponent.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/Utility/FPPathManager.h"
#include "ForestPearlEngine/Libraries/nlohmann/json.hpp"
#include "CannonFireFX.h"
#include "CannonBall.h"
#include <fstream>
#include <iostream>
#include <string>

static const char* MUZZLE_FILE = "Model/KM/frigate/Cannon_Muzzles.json";
static const FPVector3 MUZZLE_FORWARD(0.0f, 1.0f, 0.0f);	//포구 앞 = 엔진 로컬 +Y (Blender 로컬 +Z)

void Frigate::Initialize()
{
	Root = new FPSceneComponent(this);
	SetRootComponent(Root);

	for (int i = MESH_FIRST; i <= MESH_LAST; ++i)
	{
		FPStaticMeshComponent* Mesh = new FPStaticMeshComponent(this, "Object_" + std::to_string(i) + "_StaticMesh");
		Mesh->SetupAttachment(Root);
		Meshes.push_back(Mesh);
	}

	LoadMuzzles();
}

//Blender (Z 위, 오른손) → 엔진 (Y 위, 왼손) : y · z 를 바꿈
//  위치   (x, y, z)        → (x, z, y)
//  회전   (w, x, y, z)     → (w, -x, -z, -y)   축을 바꾸면 손 방향이 뒤집혀 회전 방향도 반대
//  "world" 값 = 배 모델 기준 (FBX 메쉬와 같은 공간). "local" 은 부모(Object_8.002) 기준이라 안 씀
void Frigate::LoadMuzzles()
{
	std::ifstream File(FPPathManager::Get().GetAssetPath(MUZZLE_FILE));
	if (!File)
	{
		std::cout << "[Frigate] muzzle file not found : " << MUZZLE_FILE << "\n";
		return;
	}

	nlohmann::json Json = nlohmann::json::parse(File, nullptr, false);
	if (Json.is_discarded() || !Json.contains("objects"))
	{
		std::cout << "[Frigate] muzzle file parse failed : " << MUZZLE_FILE << "\n";
		return;
	}

	for (const nlohmann::json& Object : Json["objects"])
	{
		const nlohmann::json& P = Object["world"]["position"];
		const nlohmann::json& Q = Object["world"]["rotation_quaternion"];	//w, x, y, z

		Muzzle M;
		M.Pos = { P[0].get<float>(), P[2].get<float>(), P[1].get<float>() };
		M.Rot.w = Q[0].get<float>();
		M.Rot.x = -Q[1].get<float>();
		M.Rot.y = -Q[3].get<float>();
		M.Rot.z = -Q[2].get<float>();
		Muzzles.push_back(M);
	}
}

void Frigate::BeginPlay()
{
	FireFX = static_cast<CannonFireFX*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "CannonFireFX"));
	Balls = static_cast<CannonBall*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "CannonBall"));

	FPActor* ShipActor = FPGameplayStatics::GetActorOfClass(GetWorld(), "Ship");
	if (ShipActor == nullptr) return;

	//레벨에 적은 월드 transform 을 유지한 채 Ship 에 붙임
	FTransform World = GetActorTransform();
	AttachToActor(ShipActor);
	SetActorLocation(World.Location);
	SetActorRotation(World.Rotation);
	SetActorScale3D(World.Scale);
}

int Frigate::Fire(API_KMCannon::ESide Side)
{
	//배의 지금 월드 transform (Ship 에 붙어 있으면 Ship 이동 · 회전이 이미 반영됨)
	FTransform Ship = GetActorTransform();

	int Count = 0;
	for (const Muzzle& M : Muzzles)
	{
		if (Side == API_KMCannon::ESide::Left && M.Pos.x >= 0.0f) continue;
		if (Side == API_KMCannon::ESide::Right && M.Pos.x < 0.0f) continue;

		//월드 = 부모(배) * 로컬 : 엔진 FPSceneComponent 와 같은 순서
		FPVector3 Scaled(M.Pos.x * Ship.Scale.x, M.Pos.y * Ship.Scale.y, M.Pos.z * Ship.Scale.z);
		FPVector3 Pos = Ship.Location + Rotate(Ship.QuaternionRotation, Scaled);
		FPVector3 Dir = Rotate(Ship.QuaternionRotation * M.Rot, MUZZLE_FORWARD).Normalize();

		if (FireFX != nullptr) FireFX->Fire(Pos, Dir);		//섬광 + 연기 + 빛, 포구마다
		if (Balls != nullptr) Balls->Launch(Pos, Dir);
		++Count;

	}

	return Count;
}
