#pragma once
#include "../../Engine/ForestPearlEngine/Object/Actor.h"

enum class EOrbType
{
	Sun,
	Planet,
	Moon
};

class FPOrb : public FPActor
{
	private:
		class MeshComponent* Mesh;

		float angle = 0;
		float ObjectSize = 80.f;
		float Speed = 20.f;
		EOrbType OrbType = EOrbType::Planet;

		int ThisObjectIndex = -1;
		int ParnetObjectIndex = -1;
		float DistanceThreshold = 0.1;
		bool bIsFill = true;

	public:
		FPOrb() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void SetOrbType(EOrbType Type);
		void SetParnetObjectIndex(int InIndex) { ParnetObjectIndex = InIndex; }
		void SetThisObjectIndex(int InIndex) { ThisObjectIndex = InIndex; }
		bool IsHitObject(struct FPVector2 InPos);

	private:
		//void Rotate(float angle);
};