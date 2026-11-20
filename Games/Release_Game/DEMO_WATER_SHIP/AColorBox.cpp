#include "AColorBox.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include <string>

void AColorBox::Initialize()
{
	std::string Name = GetName();
	std::string MeshName = "Cube_StaticMesh";

	if (Name.find("Red") != std::string::npos) MeshName = "ColorBox_Red_StaticMesh";
	else if (Name.find("Green") != std::string::npos) MeshName = "ColorBox_Green_StaticMesh";
	else if (Name.find("Blue") != std::string::npos) MeshName = "ColorBox_Blue_StaticMesh";
	else if (Name.find("Orange") != std::string::npos) MeshName = "ColorBox_Orange_StaticMesh";

	BoxMesh = new FPStaticMeshComponent(this, MeshName);
	BoxMesh->SetupAttachment(RootComponent);
}

void AColorBox::BeginPlay()
{
}

void AColorBox::Tick()
{
	__super::Tick();
}
