#include "Actor.h"
#include "../FPSceneComponent.h"

bool FPActor::AttachToComponent(FPSceneComponent* Parent)
{
    if (RootComponent == nullptr) return false;
    RootComponent->SetupAttachment(Parent);
    return true;
}

bool FPActor::AttachToActor(FPActor* Parent)
{
    if (RootComponent == nullptr) return false;
    RootComponent->SetupAttachment(Parent->RootComponent);
    return true;
}

bool FPActor::DetachFromActor()
{
    RootComponent->DetachFromComponent();
    return true;
}

bool FPActor::SetRootComponent(FPSceneComponent* Component)
{
    if (RootComponent != nullptr)
    {
        RootComponent->SetupAttachment(Component);
    }
    RootComponent = Component;
    return true;
}
