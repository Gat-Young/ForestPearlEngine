#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPMeshComponent;

class Player : public FPActor
{
	private:
		FPMeshComponent* Mesh;

		bool isFill = true;
		bool isCull = true;

		float angle = 0;

		float AngleSpeed = 0.25f;

	public:
		Player() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(FInputValue value);
		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
};