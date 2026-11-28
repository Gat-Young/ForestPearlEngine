#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPSceneComponent;
class FPStaticMeshComponent;

//////////////////////////////////////////////////////////////////////////////
//
// 포탄 : 총구에서 튀어나가 중력으로 포물선을 그리며 날아감
//
//  - 레벨 JSON 에 한 개 놓는 월드 액터 (CannonFireFX 와 같은 방식 : 월드 원점, 자식 상대 위치 = 월드 위치)
//  - 대포가 Launch(총구 위치, 방향) 을 부름
//  - 풀 : BALL_MAX 개를 돌려씀 (땅에 닿거나 수명 끝 → SetActive(false) → 다음 발사에 재사용)
//
//  매 프레임 (오일러 적분)
//    v ← v + g·dt        (g = (0, -9.8, 0))
//    p ← p + v·dt
//
class CannonBall : public FPActor
{
	private:
		struct Ball
		{
			FPStaticMeshComponent* Mesh = nullptr;
			FPVector3 Pos, Vel;		//위치, 속도 (m/s)
			float Age = 0;			//쏜 뒤 지난 시간 (초)
			bool  bLive = false;
		};

		static constexpr int BALL_MAX = 48;	//배 포구 34개 일제 사격 + 여유

		FPSceneComponent* Root = nullptr;		//월드 원점
		Ball Balls[BALL_MAX];

	public:
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		//쏜 순간 한 번 : Pos = 총구 (월드), Dir = 포신 방향 (월드, 정규화). 빈 칸 없으면 무시
		void Launch(const FPVector3& Pos, const FPVector3& Dir);
};
