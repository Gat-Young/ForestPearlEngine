#pragma once
#include "ForestPearlEngine/Object/Actor.h"

class MeshComponent;

class Terrain : public FPActor
{
	private:
		MeshComponent* Mesh;

	public:
		Terrain() = default;

		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;
};