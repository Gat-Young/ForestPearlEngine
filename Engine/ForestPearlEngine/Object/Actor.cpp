#include "Actor.h"
#include "../FPSceneComponent.h"
#include <typeinfo>

FPActor::FPActor()
{
    RootComponent = new FPSceneComponent(this);
}

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

bool FPActor::AttachToActor(FPActor* Parent, std::string SocketName)
{
    if (RootComponent == nullptr) return false;
    RootComponent->SetupAttachment(Parent->RootComponent, SocketName);
    return true;
}

bool FPActor::DetachFromActor()
{
    RootComponent->DetachFromComponent();
    return true;
}

bool FPActor::SetRootComponent(FPSceneComponent* Component)
{
    //Default SceneComponent 지우고 설정, 아니라면 교체
    Component->SetWorldLocation(RootComponent->GetComponentLocation());
    Component->SetWorldRotation(RootComponent->GetComponentRotation());
    Component->SetWorldScale3D(RootComponent->GetComponentScale());

    if (typeid(*RootComponent) == typeid(FPSceneComponent))
    {
        delete(RootComponent);
    }
    else
    {
        Component->DetachFromComponent();
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
    RootComponent->SetWorldRotation(Rotation);
}

void FPActor::SetActorScale3D(FPVector3 Scale)
{
    RootComponent->SetWorldScale3D(Scale);
}

FTransform FPActor::GetActorTransform()
{
    return RootComponent->GetComponentTransform();
}

FPVector3 FPActor::GetActorLocation()
{
    return RootComponent->GetComponentLocation();
}

FPVector3 FPActor::GetActorRotation()
{
    return RootComponent->GetComponentRotation();
}

FPVector3 FPActor::GetActorScale3D()
{
    return RootComponent->GetComponentScale();
}
