#include "FPSceneComponent.h"
#include <algorithm>

void FPSceneComponent::SetupAttachment(FPSceneComponent* Parent)
{
	DetachFromComponent();
	ParentComponent = Parent;
}

void FPSceneComponent::DetachFromComponent()
{
	if (ParentComponent == nullptr) return;
	ParentComponent->DetachChildComponet(this);
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