#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"

class Triangle : public FPActor
{
	private:
		class MeshComponent* Mesh;
		class TransformCompoenent* Transform;

	public:
		Triangle() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void Move(struct FInputValue value);
};