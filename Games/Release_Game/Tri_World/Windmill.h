#pragma once

#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPMeshComponent;

class Windmill : public FPActor
{
	private:
		FPMeshComponent* Body;

		float ScaleOffset = 1.0f;
		bool isFill = true;
		bool isCull = false;

	public:
		Windmill() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue Value);
		void Rotate(FInputValue Value);
		void Scaling(FInputValue Value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
};