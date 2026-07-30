#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class FPMeshComponent;

class Terrain : public FPActor
{
	private:
		FPMeshComponent* Mesh;

	public:
		Terrain() = default;

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
};