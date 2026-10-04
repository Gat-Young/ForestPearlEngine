#pragma once
#include "FPActorComponent.h"
#include "FTransform.h"

////////////////////////////////////////////////////////
//지오메트리 표현이 필요하지 않은 위치 기반 동작을 지원
//스피링 암, 카메라, 물리적 힘, 컨스트레인트, 오디오 등 포함
class FPSceneComponent : public FPActorComponent
{
	protected:
		FTransform WorldTransform;
		FTransform RelativeTransform;
		
		FPSceneComponent* ParentComponent = nullptr;
		std::vector<FPSceneComponent*> ChildComponent;

		//Parent Socket
		//비어 있다면 Socket을 사용하지 않는 것
		std::string ParentSocketName = "";

		//월드 트랜스폼 계산
		void CalculateWorldTransform();
		void CalculateWorldScale(const FTransform& ParentWorldTransform);
		void CalculateWorldRotation(const FTransform& ParentWorldTransform);
		void CalculateWorldLocation(const FTransform& ParentWorldTransform);

		//로컬 트랜스폼 계산
		void CalculateLocalTransform();

		void AttachChildComponent(FPSceneComponent* Child);
		void DetachChildComponent(FPSceneComponent* Child);

		//Tick에서 트랜스폼 계산을 위한 분기용 함수 (필요에 따라 상속한 컴포넌트에서 분기를 구현)
		virtual void CalculateTransformBranch();

	public:
		FPSceneComponent(FPActor* Owner);
		void SetupAttachment(FPSceneComponent* Parent, std::string SocketName = "");
		void DetachFromComponent();

		//이동 시키기
		void SetRelativeLocation(FPVector3 Location);
		void SetRelativeRotation(FPVector3 Rotation);
		void SetRelativeScale3D(FPVector3 Scale);

		void SetWorldLocation(FPVector3 Location);
		void SetWorldRotation(FPVector3 Rotation);
		void SetWorldScale3D(FPVector3 Scale);

		//부모 기준 변화량 추가
		void AddRelativeLocation(FPVector3 Offset);
		void AddRelativeRotation(FPVector3 Rotation);

		//자신 기준 변화량 추가
		void AddLocalOffset(FPVector3 Offset);
		void AddLocalRotation(FPVector3 Rotation);

		//월드 기준 변화량 추가
		void AddWorldOffset(FPVector3 Offset);
		void AddWorldRotation(FPVector3 Rotation);

		//Transform 가져오기
		FTransform GetComponentTransform();
		FTransform GetRelativeTransform();
		//Relative
		FPVector3 GetRelativeLocation();
		FPVector3 GetRelativeRotation();
		FPVector3 GetRelativeScale3D();

		//World
		FPVector3 GetComponentLocation();
		FPVector3 GetComponentRotation();
		FPQuaternion GetComponentQuat();
		FPVector3 GetComponentScale();

		//Root Component 찾기
		FPSceneComponent* GetAttachmentRoot();

		//Socekt
		virtual FTransform GetSocketTransform(const std::string& SocketName) const;

		//임시로 사용
		virtual void Tick();
};