#include "FPStaticMesh.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"
#include "FPMaterial.h"
#include <iostream>

FPStaticMesh::FPStaticMesh(std::string StaticMeshPath, std::string InNodeName) : MeshPath(StaticMeshPath), NodeName(InNodeName)
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

std::string* FPStaticMesh::GetNodeName()
{
	return &NodeName;
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

void FPStaticMesh::SetStencilRefValue(int StencilRefValue)
{
	this->StencilRefValue = StencilRefValue;
}

int FPStaticMesh::GetStencilRefValue()
{
	return this->StencilRefValue;
}

void FPStaticMesh::SetCustomDepthStencil(bool bCustomDepthStencil)
{
	this->bCustomDepthStencil = bCustomDepthStencil;
}

bool FPStaticMesh::GetCustomDepthStencil()
{
	return this->bCustomDepthStencil;
}

void FPStaticMesh::SetBoundingBox(FPAABB BoundingBox)
{
	this->BoundingBox = BoundingBox;

	//BoundingBox(AABB)로부터 BoxSphereBounds도 함께 계산
	this->Bounds = BoundsFromAABB(BoundingBox);
}

FPAABB FPStaticMesh::GetBoundingBox()
{
	return this->BoundingBox;
}

FPBoxSphereBounds FPStaticMesh::GetBounds()
{
	return this->Bounds;
}
