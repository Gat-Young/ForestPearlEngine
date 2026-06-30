#include "FPSceneComponent.h"
#include <algorithm>
#include <iostream>

FPSceneComponent::FPSceneComponent(FPActor* Owner) : FPActorComponent(Owner)
{
}

void FPSceneComponent::SetupAttachment(FPSceneComponent* Parent)
{
	DetachFromComponent();
	ParentComponent = Parent;
	ParentComponent->AttachChildComponent(this);
}

void FPSceneComponent::DetachFromComponent()
{
	if (ParentComponent == nullptr) return;
	ParentComponent->DetachChildComponent(this);
}

void FPSceneComponent::SetRelativeLocation(FPVector3 Location)
{
	RelativeTransform.Location = Location;
}

void FPSceneComponent::SetRelativeRotation(FPVector3 Rotation)
{
	RelativeTransform.Rotation = Rotation;
}

void FPSceneComponent::SetRelativeScale3D(FPVector3 Scale)
{
	RelativeTransform.Scale = Scale;
}

void FPSceneComponent::SetWorldLocation(FPVector3 Location)
{
	SetRelativeLocation(Location);
	WorldTransform.Location = Location;
}

void FPSceneComponent::SetWorldRotation(FPVector3 Rotation)
{
	SetRelativeRotation(Rotation);
	WorldTransform.Rotation = Rotation;
}

void FPSceneComponent::SetWorldScale3D(FPVector3 Scale)
{
	SetRelativeScale3D(Scale);
	WorldTransform.Scale = Scale;
}

FTransform FPSceneComponent::GetComponentTransform()
{
	return WorldTransform;
}

FPVector3 FPSceneComponent::GetComponentLocation()
{
	return WorldTransform.Location;
}

FPVector3 FPSceneComponent::GetComponentRotation()
{
	return WorldTransform.Rotation;
}

FPQuaternion FPSceneComponent::GetComponentQuat()
{
	return WorldTransform.QuaternionRotation;
}

FPVector3 FPSceneComponent::GetComponentScale()
{
	return WorldTransform.Scale;
}

FTransform FPSceneComponent::GetRelativeTransform()
{
	return RelativeTransform;
}

FPVector3 FPSceneComponent::GetRelativeLocation()
{
	return RelativeTransform.Location;
}

FPVector3 FPSceneComponent::GetRelativeRotation()
{
	return RelativeTransform.Rotation;
}

FPVector3 FPSceneComponent::GetRelativeScale3D()
{
	return RelativeTransform.Scale;
}

//임시로 사용
void FPSceneComponent::Tick()
{
	RelativeTransform.QuaternionRotation = FromEuler(RelativeTransform.Rotation).Normalize();

	if (ParentComponent == nullptr)
	{
		WorldTransform = RelativeTransform;
	}
	else
	{
		WorldTransform.Scale = ParentComponent->WorldTransform.Scale * RelativeTransform.Scale;

		WorldTransform.QuaternionRotation = RelativeTransform.QuaternionRotation;//ParentComponent->WorldTransform.QuaternionRotation * RelativeTransform.QuaternionRotation; //부모의 회전까지 영향 받게 하고 싶다면

		WorldTransform.Location = ParentComponent->WorldTransform.Location + Rotate(ParentComponent->WorldTransform.QuaternionRotation 
			                                                                        ,(ParentComponent->WorldTransform.Scale * RelativeTransform.Location));
	}

	WorldTransform.Rotation = WorldTransform.QuaternionRotation.ToEuler();


	for (FPSceneComponent* child : ChildComponent)
	{
		child->Tick();
	}
}

void FPSceneComponent::AttachChildComponent(FPSceneComponent* Child)
{
	ChildComponent.push_back(Child);
}

void FPSceneComponent::DetachChildComponent(FPSceneComponent* Child)
{
	auto It = std::find(ChildComponent.begin(), ChildComponent.end(), Child);

	if (It != ChildComponent.end())
	{
		std::cout << "erase" << "\n";
		ChildComponent.erase(It);
	}
	else
	{
		return;
	}
}