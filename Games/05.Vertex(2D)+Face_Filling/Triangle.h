#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"
#include "../../Engine/ForestPearlEngine/Systems/KeyStateEnum.h"
#include "../../Engine/ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;

class Triangle : public FPActor
{
	private:
		MeshComponent* Mesh;
		FPInputMappingContext* IMC;
		FPInputAction* IA = nullptr;

		bool isFill = true;

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FPVector2 value);
		void SetFillTriangel(FPVector2 Value);
};