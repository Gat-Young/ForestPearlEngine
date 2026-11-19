#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPStaticMeshComponent;
class CannonFireFX;
class CannonBall;

class Cannon : public FPActor
{
private:
	FPStaticMeshComponent* Mesh;
	FPActor* Muzzle = nullptr;		//총구 소켓 : 레벨에 놓인 CannonSocket "<대포이름>_Muzzle" 을 BeginPlay 에서 대포에 붙임 (포구 방향 +Z)
	CannonFireFX* FireFX = nullptr;	//발사 이펙트 (레벨에 놓인 월드 액터)
	CannonBall* Balls = nullptr;		//포탄 풀 (레벨에 놓인 월드 액터)

public:
	virtual void Initialize() override;
	virtual void BeginPlay() override;

	void Fire();					//쏘기 : 이펙트 + 포탄
};

//★ 소켓 = 그림 없는 빈 액터 (위치 · 회전만). 레벨 JSON 에 월드 좌표로 놓고, 주인이 BeginPlay 에서 자기에게 붙임
class CannonSocket : public FPActor
{
public:
	virtual void Initialize() override {}
	virtual void BeginPlay() override {}
};
