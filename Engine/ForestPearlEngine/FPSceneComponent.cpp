#include "FPSceneComponent.h"
#include <algorithm>

void FPSceneComponent::SetupAttachment(FPSceneComponent* Parent)
{
	DetachFromComponent();
	ParentComponent = Parent;
	ParentComponent->AttachChildCompont(this);
}

void FPSceneComponent::DetachFromComponent()
{
	if (ParentComponent == nullptr) return;
	ParentComponent->DetachChildComponet(this);
}

//임시로 사용
void FPSceneComponent::Tick()
{
	if (ParentComponent == nullptr)
	{
		WorldTransform = RelativeTransform;
	}
	else
	{
		WorldTransform.Scale = ParentComponent->WorldTransform.Scale * RelativeTransform.Scale;

		WorldTransform.QuaternionRotation = ParentComponent->WorldTransform.QuaternionRotation * RelativeTransform.QuaternionRotation;

		WorldTransform.Location = ParentComponent->WorldTransform.Location + Rotate(ParentComponent->WorldTransform.QuaternionRotation 
			                                                                        ,(ParentComponent->WorldTransform.Scale * RelativeTransform.Location));
	}

	RelativeTransform.QuaternionRotation = FromEuler(RelativeTransform.Rotation);
	WorldTransform.Rotation = WorldTransform.QuaternionRotation.ToEuler();

	for (FPSceneComponent* child : ChildComponent)
	{
		child->Tick();
	}
}

void FPSceneComponent::AttachChildCompont(FPSceneComponent* Child)
{
	ChildComponent.push_back(Child);
}

void FPSceneComponent::DetachChildComponet(FPSceneComponent* Child)
{

	auto It = std::find(ChildComponent.begin(), ChildComponent.end(), Child);

	if (It != ChildComponent.end())
	{
		ChildComponent.erase(It);
	}
	else
	{
		return;
	}
}