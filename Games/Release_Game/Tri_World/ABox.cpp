#include "ABox.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void ABox::Initialize()
{
	BoxMesh = new FPStaticMeshComponent(this, "Box_StaticMesh");
	BoxMesh->SetupAttachment(RootComponent);
}

void ABox::BeginPlay()
{

}

void ABox::Tick()
{
	__super::Tick();
}

void ABox::ChangeSeason(int SeasonCnt)
{
	BoxMesh->GetMaterial().SetParameter(FPShaderType::PS, "SeasonConstBuffer", "SeasonCnt", static_cast<float>(SeasonCnt));

}

