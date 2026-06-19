#include "Actor.h"
#include "../FPSceneComponent.h"

//임시로 컴포넌트 Tick 수행
void FPActor::Tick()
{
    if (RootComponent != nullptr) RootComponent->Tick();
}

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

void FPActor::SetActorLocation(FPVector3 Location)
{
    RootComponent->SetWorldLocation(Location);
}

void FPActor::SetActorRotation(FPVector3 Rotation)
{
    RootComponent->SetRelativeRotation(Rotation);
}

void FPActor::SetActorScale3D(FPVector3 Scale)
{
    RootComponent->SetWorldScale3D(Scale);
}
