#pragma once
#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"
#include "ForestPearlEngine/FTransform.h"
#include <vector>

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;
class FPSpringArmComponent;
class FPCameraComponent;

class Ship : public FPPawn
{
private:
	FTransform InitialShipTransform;	// 처음 배 위치·회전·크기
	FTransform InitialArmTransform;		// 처음 스프링암 상대 위치·회전·크기
	float InitialArmLength = 0.0f;	// 처음 카메라 거리

	// CameraMove에서 변경한 컨트롤러 회전량
	float CameraYawOffset = 0.0f;	// 시작 후 변경한 좌우 시선 회전량
	float CameraPitchOffset = 0.0f;	// 시작 후 변경한 상하 시선 회전량
private:

	//JosenShip Model
	std::vector<FPStaticMeshComponent*> JosenShipMeshes;

	FPSceneComponent* ShieldPivot;
	FPSpringArmComponent* SpringArm;
	FPCameraComponent* PlayerCamera;

	float angle = 0;

	float AngleSpeed = 0.25f;

public:
	void ResetView(FInputValue Value);
public:
	Ship() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;

	void Move(FInputValue value);
	void CameraMove(FInputValue value);
	void CameraZoom(FInputValue Value);
};