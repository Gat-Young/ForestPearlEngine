#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPSceneComponent;
class FPStaticMeshComponent;
class FPPointLightComponent;

//////////////////////////////////////////////////////////////////////////////
//
// 대포 발사 이펙트 : 연기 + 총구 섬광   (Cena Particle.cpp 의 ParticleBurst · ParticleMuzzle · LightFlash)
//
//  - 레벨 JSON 에 한 개 놓는 월드 액터 (배에 안 붙음 → 쏜 뒤 배가 움직여도 연기는 그 자리에 남음)
//  - 대포가 Fire(총구 위치, 방향) 을 부름
//  - 판 1장 = FPStaticMeshComponent 1개 (Plane 메쉬 + 빌보드 셰이더), 판마다 머티리얼 따로 (색 · 크기가 다르므로)
//
//  1. 풀   : SMOKE_MAX 칸을 돌려씀 (수명 끝 → SetActive(false) → 다음 발사에 재사용)
//  2. 연기 : 알파 섞기 · 깊이 쓰기 끔, 주황 → 회색, 퍼지며 사라짐
//  3. 섬광 : 더하기 섞기, 0.1초 동안 정면 별(보통 빌보드) + 옆 불꽃(늘인 빌보드) → 어느 각도에서 봐도 한쪽은 보임
//  4. 빛   : 점광원 0.3초, 세기 5 → 0 (주변 배 · 대포가 순간 밝아짐)
//
class CannonFireFX : public FPActor
{
	private:
		struct Puff
		{
			FPStaticMeshComponent* Mesh = nullptr;
			FPVector3 Pos, Vel;			//위치, 속도 (m/s)
			float Age = 0, Life = 1;	//지난 시간, 수명 (초)
			float Size = 1, Rot = 0, Spin = 0;
			bool  bLive = false;
		};

		static constexpr int SMOKE_MAX = 32;

		FPSceneComponent* Root = nullptr;			//월드 원점 : 자식들의 상대 위치 = 월드 위치
		Puff Smoke[SMOKE_MAX];

		FPStaticMeshComponent* FlashFront = nullptr;	//정면 별
		FPStaticMeshComponent* FlashSide = nullptr;		//옆 불꽃
		FPPointLightComponent* FlashLight = nullptr;

		float MuzzleT = 0;		//섬광 남은 시간 : > 0 이면 그림
		float LightT = 0;		//빛 남은 시간
		float MuzzleRot = 0;	//정면 별 각도 : 쏠 때마다 다르게
		FPVector3 MuzzlePos, MuzzleDir;

		FPStaticMeshComponent* MakeQuad(const char* MaterialName, int Priority);
		Puff* EmitSmoke(const FPVector3& Pos);
		void UpdateSmoke(float dTime);
		void UpdateMuzzle(float dTime);

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		//쏜 순간 한 번 : Pos = 총구 (월드), Dir = 포신 방향 (월드, 정규화)
		void Fire(const FPVector3& Pos, const FPVector3& Dir);
};
