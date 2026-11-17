#include "ASkyBox.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

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

void ASkyBox::SetEnvironmentMap(std::string TextureName)
{
	SkyBoxMesh->GetMaterial().SetNamedTexture(FPShaderType::PS, "EnvTex", TextureName, FPTextureType::CubeMap, 0);
}
