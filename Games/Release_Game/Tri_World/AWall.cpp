#include "AWall.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"

void AWall::Initialize()
{
	WallMesh = new FPStaticMeshComponent(this, "Plane_StaticMesh");
	WallMesh->SetupAttachment(RootComponent);
	WallMesh->SetRelativeRotation({ -90.0f, 0.0f, 0.0f });

	//WallDepthPass로 보내기 위한 표시 (StencilRefValue 1 = Wall occluder, 8 = ToonLink)
	WallMesh->SetStencilRefValue(1);
	WallMesh->SetCustomDepthStencil(true);
}

void AWall::BeginPlay()
{

}

void AWall::Tick()
{
	__super::Tick();
}
