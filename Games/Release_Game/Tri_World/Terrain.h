#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;

class Terrain : public FPActor
{
	private:
		FPStaticMeshComponent* Mesh;

		bool isFill = true;
		bool isCull = false;


	public:
		Terrain() = default;

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void SetFillTriangel(FInputValue Value);
		void SetCullTriangle(FInputValue Value);
};