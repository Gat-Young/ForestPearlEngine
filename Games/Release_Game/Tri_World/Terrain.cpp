#include "Terrain.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void Terrain::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Terrain_StaticMesh");

	//터레인은 Shadow Map에 찍히지 않도록 함 (그림자를 받을 수는 있음)
	Mesh->SetCastShadow(false);

	SetRootComponent((FPSceneComponent*)Mesh);

}

void Terrain::BeginPlay()
{

}

void Terrain::Tick()
{
	__super::Tick();
}


void Terrain::ChangeSeason(int SeasonCnt)
{
	Mesh->GetMaterial().SetParameter(FPShaderType::PS, "SeasonConstBuffer", "SeasonCnt", static_cast<float>(SeasonCnt));
}
