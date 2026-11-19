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
//  1. 풀   : 연기 SMOKE_MAX 칸 · 섬광 FLASH_MAX 칸을 돌려씀 (수명 끝 → SetActive(false) → 다음 발사에 재사용)
//           일제 사격(배 포구 34개) 때도 포구마다 자기 자리 · 방향에 섬광 + 연기
//  2. 연기 : 알파 섞기 · 깊이 쓰기 끔, 주황 → 회색, 퍼지며 사라짐
//  3. 섬광 : 더하기 섞기, 0.1초 동안 정면 별(보통 빌보드) + 옆 불꽃(늘인 빌보드) → 어느 각도에서 봐도 한쪽은 보임
//  4. 빛   : 점광원 하나 0.3초, 세기 5 → 0, 마지막에 쏜 포구 자리 (엔진 조명 개수 한도 때문에 포구마다 두지 않음)
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

		struct Flash
		{
			FPStaticMeshComponent* Front = nullptr;	//정면 별
			FPStaticMeshComponent* Side = nullptr;	//옆 불꽃
			FPVector3 Pos, Dir;
			float T = 0;			//남은 시간 : > 0 이면 그림
			float Rot = 0;			//정면 별 각도 : 쏠 때마다 다르게
		};

		static constexpr int SMOKE_MAX = 448;	//포구 34 × 연기 12 + 여유
		static constexpr int FLASH_MAX = 40;	//포구 34 + 여유

		FPSceneComponent* Root = nullptr;			//월드 원점 : 자식들의 상대 위치 = 월드 위치
		Puff Smoke[SMOKE_MAX];

		Flash Flashes[FLASH_MAX];
		FPPointLightComponent* FlashLight = nullptr;
		float LightT = 0;		//빛 남은 시간

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
