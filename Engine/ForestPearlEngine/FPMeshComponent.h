#pragma once
#include "FPMeshRenderList.h"
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"
#include <string>

class FPMeshComponent : public FPPrimitiveComponent
{
	private:
		std::string MeshData;

		int Priority = 0;
		std::vector<void*> VB;
		std::vector<int> VertexSize;
		int Stride;
		int Offest;
		bool isFill = true;
		bool isCull = true;
		bool isActive = true;
		Topology Topo = TRIANGLELIST;
		FPMaterial* Material = nullptr;

		RenderItem* RenderItem = nullptr;

		std::vector<struct FPVertexBufferData> LoadVertexBuffer(std::string MeshPath);
		void RegistMeshRenderList();

	public:
		FPMeshComponent(FPActor* Owner, std::string MeshPath);

		~FPMeshComponent();

		void SetMeshFill(bool State) { isFill = State; };
		void SetMeshCull(bool State) { isCull = State; };
		void SetActive(bool Active) { this->isActive = Active; }
		void SetTopology(Topology State) { Topo = State; };
		void SetPriority(int Prio) { Priority = Prio; };
		void SetMaterial(FPMaterial* Material);
};