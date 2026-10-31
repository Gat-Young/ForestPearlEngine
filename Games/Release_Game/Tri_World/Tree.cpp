#include "Tree.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPMaterialInterface.h"

void Tree::Initialize()
{
	TreeLeaf = new FPStaticMeshComponent(this, "Tree_StaticMesh");
	TreeLeaf->SetupAttachment(RootComponent);
	TreeTrunk = new FPStaticMeshComponent(this, "Tree_Trunk_StaticMesh");
	TreeTrunk->SetupAttachment(RootComponent);


}

void Tree::BeginPlay()
{

}

void Tree::Tick()
{
	__super::Tick();
}

void Tree::ChangeSeason(int SeasonCnt)
{
	TreeLeaf->GetMaterial().SetParameter(FPShaderType::PS, "SeasonConstBuffer", "SeasonCnt", static_cast<float>(SeasonCnt));
	TreeTrunk->GetMaterial().SetParameter(FPShaderType::PS, "SeasonConstBuffer", "SeasonCnt", static_cast<float>(SeasonCnt));
}
