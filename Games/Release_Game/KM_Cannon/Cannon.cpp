#include "Cannon.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"

void Cannon::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Canon_1_StaticMesh");
	//Mesh->SetupAttachment(RootComponent); //루트에 자식
	SetRootComponent(Mesh); //내가 루트
}

void Cannon::BeginPlay()
{
}