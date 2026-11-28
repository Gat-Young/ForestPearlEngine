#include "ASkyBox.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"

void ASkyBox::Initialize()
{
	SkyBoxMesh = new FPStaticMeshComponent(this, "SkyBoxQuad_StaticMesh");
	SkyBoxMesh->SetupAttachment(RootComponent);
	SkyBoxMesh->SetPriority(-999);
}

void ASkyBox::BeginPlay()
{
}

void ASkyBox::Tick()
{
	__super::Tick();
}
