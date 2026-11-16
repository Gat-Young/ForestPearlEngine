#include "AUtaTeapot.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"

void AUtaTeapot::Initialize()
{
	TeapotUp = new FPStaticMeshComponent(this, "UtaTeapot_Up_StaticMesh");
	TeapotUp->SetupAttachment(RootComponent);

	TeapotBottom = new FPStaticMeshComponent(this, "UtaTeapot_Bottom_StaticMesh");
	TeapotBottom->SetupAttachment(RootComponent);
}

void AUtaTeapot::BeginPlay()
{

}

void AUtaTeapot::Tick()
{
	__super::Tick();
}
