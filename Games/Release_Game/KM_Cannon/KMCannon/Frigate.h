#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Define/FPMath.h"
#include "API_KMCannon.h"
#include <vector>

class FPSceneComponent;
class FPStaticMeshComponent;
class CannonFireFX;
class CannonBall;

//////////////////////////////////////////////////////////////////////////////
//
// 배 모델 (frigate.fbx) : 노드 41개 = StaticMesh 41개 (Object_2 ~ Object_42) 를 한 루트에 붙여 그림
//
//  - 위치 · 회전 · 크기는 레벨 JSON 의 transform (다시 빌드 안 해도 됨)
//  - 레벨에 Ship(조종하는 폰) 이 있으면 BeginPlay 에서 거기에 붙음 → 배를 움직이면 같이 감
//  - 포구 34개 : Assets/Model/KM/frigate/Cannon_Muzzles.json (Blender 에서 내보낸 값) 을 읽어 "배 모델 기준" 으로 저장
//    소켓(컴포넌트) 없이, 쏠 때마다 배의 지금 월드 transform 을 곱해 월드 위치 · 방향을 구함 → 배가 움직여도 따라감
//
class Frigate : public FPActor
{
	public:
		static constexpr int MESH_FIRST = 2, MESH_LAST = 42;	//Object_2_StaticMesh ~ Object_42_StaticMesh
		static constexpr int IMAGE_COUNT = 64;				//Image_0 ~ Image_63

	private:
		struct Muzzle
		{
			FPVector3 Pos;			//배 모델 기준 위치 (엔진 축)
			FPQuaternion Rot;		//배 모델 기준 회전 (엔진 축), 앞 = 로컬 +Y
		};

		FPSceneComponent* Root = nullptr;
		std::vector<FPStaticMeshComponent*> Meshes;
		std::vector<Muzzle> Muzzles;
		CannonFireFX* FireFX = nullptr;
		CannonBall* Balls = nullptr;

		void LoadMuzzles();

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;

		int Fire(API_KMCannon::ESide Side);	//해당 뱃전 포구에서 발사 (이펙트 + 포탄), 쏜 개수 반환
};
