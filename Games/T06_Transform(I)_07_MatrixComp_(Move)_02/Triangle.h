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

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
};