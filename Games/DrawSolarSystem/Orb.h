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
		class TransformCompoenent* FPTransform;
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
		TransformCompoenent& GetTransform() { return *FPTransform; }
		void SetParnetObjectIndex(int InIndex) { ParnetObjectIndex = InIndex; }
		void SetThisObjectIndex(int InIndex) { ThisObjectIndex = InIndex; }
		bool IsHitObject(struct FPVector2 InPos);
		FPVector2 GetWorldPos();

		//void SetPosition(const MYHelper::Vector2F& pos);
		//bool IsHitTest(D2D1_POINT_2F WorldPoint, D2D1::Matrix3x2F ViewTM);
		//void SetRelationship(MObject* Parent);

		//D2DTM::Transform* GetTransform() { return &Transform; }
		//void SetParent(bool bInIsParent) { bIsParent = bInIsParent; }
		//EObjectType GetObjectType() { return ObjectType; }
		//float GetObjectSize() { return ObjectSize; }

	private:
		//void Rotate(float angle);
};