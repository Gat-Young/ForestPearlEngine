#pragma once
#include "FPSceneComponent.h"
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPCameraList.h"

class FPMaterialInterface;

class FPCameraComponent : public FPSceneComponent
{
	public:
		//카메라 속성
		FPVector3 LookAt = {0.0f, 0.0f, 1.0f};		//바라보는 곳. 위치
		FPVector3 Up = {0.0f, 1.0f, 0.0f};			//카메라 상방 벡터

		//투영 변환 용
		float Fov = DegToRad(45.0f);							//시야각			
		float Aspect = 960.0f / 600.0f;				//가로:세로 비율
		float Zn = 1.0f;							//근평면 거리
		float Zf = 300.0f;							//원평면 거리

		FPMatrix ViewMatrix;
		FPMatrix ProjectionMatrix;

		bool Active = true;			//카메라 사용 여부

		//PostProcess Material. nullptr이면 PostProcess를 사용하지 않음 (Camera가 소유)
		FPMaterialInterface* PostProcessMaterial = nullptr;

		CameraItem* CamItem = nullptr;

	public:
		FPCameraComponent(FPActor* Owner);
		~FPCameraComponent();
		void RegistCamera();
		void Tick() override;

		//PostProcess (MeshRenderPass 이후 호출되는 PostProcessRenderPass가 이 Material을 사용)
		//Material 이름으로 PostProcess Material을 생성해 설정 (기존 Material은 해제, 빈 문자열이나 없는 이름이면 해제만 함)
		void SetPostProcessMaterial(std::string MaterialName);

		//PostProcess Material (런타임에 SetParameter 등으로 값 변경 가능, 없으면 nullptr)
		FPMaterialInterface* GetPostProcessMaterial() const;
};