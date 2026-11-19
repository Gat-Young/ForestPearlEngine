#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include <vector>

class FPSceneComponent;
class FPStaticMeshComponent;

//////////////////////////////////////////////////////////////////////////////
//
// 배 모델 (frigate.fbx) : 노드 41개 = StaticMesh 41개 (Object_2 ~ Object_42) 를 한 루트에 붙여 그림
//
//  - 위치 · 회전 · 크기는 레벨 JSON 의 transform (다시 빌드 안 해도 됨)
//  - 레벨에 Ship(조종하는 폰) 이 있으면 BeginPlay 에서 거기에 붙음 → 배를 움직이면 같이 감
//
class Frigate : public FPActor
{
	public:
		static constexpr int MESH_FIRST = 2, MESH_LAST = 42;	//Object_2_StaticMesh ~ Object_42_StaticMesh
		static constexpr int IMAGE_COUNT = 64;				//Image_0 ~ Image_63

	private:
		FPSceneComponent* Root = nullptr;
		std::vector<FPStaticMeshComponent*> Meshes;

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
};
