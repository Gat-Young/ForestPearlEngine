#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Systems/KeyStateEnum.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"

struct FInputValue;

class Triangle : public FPActor
{
	private:
		
		MeshComponent* Mesh;

		bool isFill = true;
		bool isCull = true;

		float angle = 0;

		float DistanceThreshold = 0.1;
		bool bIsFill = true;
		
		float AngleSpeed = 0.25f;

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);

		bool IsHitObject(struct FPVector2 InPos);

		void SetAngleSpeed(float Speed) { AngleSpeed = Speed; };
};