#include "FPStaticMesh.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"
#include "FPMaterial.h"
#include <iostream>

FPStaticMesh::FPStaticMesh(std::string StaticMeshPath) : MeshPath(StaticMeshPath)
{
	if (Material == nullptr) { Material = new FPMaterial(); }
}

void FPStaticMesh::AddSocketData(std::string SocketName, FTransform SocketTransform)
{
	if (Sockets.find(SocketName) == Sockets.end())
	{
		Sockets[SocketName] = SocketTransform;
	}
	else
	{
		std::cout << "[StaticMesh] : 이미 있는 SocketName 입니다 : " << SocketName << "\n";
	}
}

std::string* FPStaticMesh::GetMeshPath()
{
	return &MeshPath;
}

bool FPStaticMesh::GetSocketTransform(std::string SocketName, FTransform& OutSocketTransform)
{
	if (Sockets.find(SocketName) != Sockets.end())
	{
		OutSocketTransform = Sockets[SocketName];
		return true;
	}
	else
	{
		//std::cout << "[StaticMesh] : 해당 이름의 Socket이 없습니다 : " << SocketName << "\n";
		return false;
	}
}

FPMaterialInterface** FPStaticMesh::GetStaticMeshMaterial()
{
	return &(this->Material);
}

void FPStaticMesh::SetMaterial(FPMaterialInterface* Material)
{
	this->Material = Material;
}
