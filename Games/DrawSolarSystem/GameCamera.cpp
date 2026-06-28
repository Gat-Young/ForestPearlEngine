#include "GameCamera.h"
#include "../../Engine/ForestPearlEngine/CameraComponent.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/FPWorld.h"
#include "../../Engine/ForestPearlEngine/GameTimer.h"

void GameCamera::Initialize()
{
	Camera = new CameraComponent();

	//카메라 위치
	Camera->Position.x = 0.0f; Camera->Position.y = 5.0f; Camera->Position.z = -10.0f;
	Camera->Rotation.x = 0.0f; Camera->Rotation.y = 0.0f; Camera->Rotation.z = 0.0f;
	Camera->Scale.x = 0.0f; Camera->Scale.y = 0.0f; Camera->Scale.z = 0.0f;

	//카메라 바라보는 곳
	Camera->LookAt.x = 0.0f; Camera->LookAt.y = 0.0f; Camera->LookAt.z = 0.0f;

	//카메라 상방 벡터
	Camera->Up.x = 0.0f; Camera->Up.y = 1.0f; Camera->Up.z = 0.0f;

	//투영변환 렌즈 설정
	Camera->Fov = 45.0f;			//시야각
	Camera->Aspect = 4.0f / 3.0f;	//가로:세로 비율
	Camera->Zn = 1.0f;				//근평면 거리
	Camera->Zf = 100.0f;			//원평면 거리

	Camera->Active = true;			//카메라 사용 설정

	Camera->RegistCamera(); //카메라 컴포넌트 등록
}

void GameCamera::BeginPlay()
{
}

void GameCamera::Tick()
{
}
