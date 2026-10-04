#pragma once
#include "FPStreamableRenderAssetr.h"
#include "Define/FPDataDefine.h"
#include "FTransform.h"
#include <map>
#include <vector>
#include <string>

class FPMaterialInterface;

class FPStaticMesh : public FPStreamableRenderAsset
{
	private:
		//Mesh Data 정보
		std::vector<void*> VB;
		std::vector<int> VertexSize;
		int Stride;
		int Offset;
		Topology Topo = Topology::TRIANGLELIST;

		//Mesh가 가지는 소켓 정보
		std::map<std::string, FTransform> Sockets;
	
		//StaticMesh가 기본적으로 가지는 Material 정보
		FPMaterialInterface* Material = nullptr;

	public:
		FPStaticMesh(std::string StaticMeshPath, std::string Topo);

		void AddSocketData(std::string SocketName, FTransform SocketTransform);
		void SetMaterial(FPMaterialInterface* Material);

		bool GetSocketTransform(std::string SocketName, FTransform& OutSocketTransform);
		std::vector<void*>* GetVBData();
		std::vector<int>* GetVertexSize();
		int* GetStride();
		int* GetOffset();
		void SetTopo(std::string Topology);
		Topology* GetTopo();
		FPMaterialInterface** GetStaticMeshMaterial();
};