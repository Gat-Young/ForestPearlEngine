#pragma once
#include "ForestPearlEngine/Object/Actor.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"
#include "ForestPearlEngine/Define/FPMath.h"

class FPInputMappingContext;
class FPInputAction;
struct FInputValue;
class FPStaticMeshComponent;

class Tree : public FPActor
{
private:
	FPStaticMeshComponent* Mesh;



public:
	Tree() = default;
	virtual void Initialize() override;
	virtual void BeginPlay() override;
	virtual void Tick() override;
};
