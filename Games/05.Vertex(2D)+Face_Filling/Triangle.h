#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"
#include "../../Engine/ForestPearlEngine/MeshComponent.h"

class Triangle : public FPActor
{
	private:
		MeshComponent* Mesh;

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
};