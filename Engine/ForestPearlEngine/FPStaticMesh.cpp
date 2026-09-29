#include "FPStaticMesh.h"
#include "FPAssetManager.h"
#include "FPGameInstance.h"
#include "FPMaterial.h"
#include <iostream>

FPStaticMesh::FPStaticMesh(std::string StaticMeshPath, std::string Topo)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	std::vector<FPVertexBufferData> MeshData = AssetManager->GetVertexBuffer(StaticMeshPath);

	for (FPVertexBufferData Mesh : MeshData)
	{
		VB.push_back(Mesh.VertexBuffer);
		VertexSize.push_back(Mesh.Size);
		Stride = Mesh.Stride;
		Offset = Mesh.Offset;
	}
	
	this->Topo = StringToTopology(Topo);

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

FTransform& FPStaticMesh::GetSocketTransform(std::string SocketName)
{
	if (Sockets.find(SocketName) != Sockets.end())
	{
		return Sockets[SocketName];
	}
	else
	{
		std::cout << "[StaticMesh] : 해당 이름의 Socket이 없습니다 : " << SocketName << "\n";
	}
}

std::vector<void*>* FPStaticMesh::GetVBData()
{
	return &(this->VB);
}

std::vector<int>* FPStaticMesh::GetVertexSize()
{
	return &(this->VertexSize);
}

int* FPStaticMesh::GetStride()
{
	return &(this->Stride);
}

int* FPStaticMesh::GetOffset()
{
	return &(this->Offset);
}

Topology* FPStaticMesh::GetTopo()
{
	return &(this->Topo);
}

FPMaterialInterface** FPStaticMesh::GetStaticMeshMaterial()
{
	return &(this->Material);
}

void FPStaticMesh::SetMaterial(FPMaterialInterface* Material)
{
	this->Material = Material;
}
