#pragma once

#include "ForestPearlEngine/Object/FPPawn.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;
class WindmillWing;
class TripleWindmillWing;

class Windmill : public FPPawn
{
	private:
		FPStaticMeshComponent* Body;
		WindmillWing* OneWindmillWing;
		TripleWindmillWing* TripleWing;

		float ScaleOffset = 1.0f;
		bool isFill = true;
		bool isCull = false;

	public:
		Windmill() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void SetOneWindmillWing(WindmillWing* Wing);
		void SetTripleWindmillWing(TripleWindmillWing* Wing);

		void Move(FInputValue Value);
		void Rotate(FInputValue Value);
		void Scaling(FInputValue Value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
		void SetScaleWing(FInputValue Value);
};