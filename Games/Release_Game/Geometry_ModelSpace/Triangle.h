#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/MeshComponent.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;

class Triangle : public FPActor
{
	private:
		MeshComponent* Mesh;

		bool isFill = true;
		bool isCull = true;

		float angle = 0;

		float AngleSpeed = 0.25f;

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
};