#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Systems/KeyStateEnum.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"
#include "../../Engine/ForestPearlEngine/TransformComponent.h"

class FPInputMappingContext;
class FPInputAction;

class Triangle : public FPActor
{
	private:
		TransformCompoenent* Transform;
		MeshComponent* Mesh;
		FPInputMappingContext* IMC;
		FPInputAction* IA = nullptr;

		FPInputMappingContext* IMC2;
		FPInputAction* IA2 = nullptr;

		bool isFill = true;
		bool isCull = true;

		float angle = 0;

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FPVector2 value);
		void SetFillTriangel(FPVector2 Value);
		void SetCullTriangle(FPVector2 Value);
};