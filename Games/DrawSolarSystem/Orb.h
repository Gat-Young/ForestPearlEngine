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
		class TransformCompoenent* Transform;
		class MeshComponent* Mesh;

		float angle = 0;
		float ObjectSize = 80.f;
		float Speed = 20.f;
		EOrbType OrbType = EOrbType::Planet;

		int ThisObjectIndex = -1;
		int ParnetObjectIndex = -1;

	public:
		FPOrb() = default;
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		void SetOrbType(EOrbType Type);
		TransformCompoenent& GetTransform() { return *Transform; }
		void SetParnetObjectIndex(int InIndex) { ParnetObjectIndex = InIndex; }
		void SetThisObjectIndex(int InIndex) { ThisObjectIndex = InIndex; }

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