#pragma once
#include "MeshRenderList.h"
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"
#include <string>

class MeshComponent : FPPrimitiveComponent
{
	private:
		std::string MeshData;

		int Priority = 0;
		std::vector<int> VBIndex;
		std::vector<int> VertexSize;
		bool isFill = true;
		bool isCull = true;
		bool isActive = true;
		Topology Topo = TRIANGLELIST;
		FPMaterial* Material = nullptr;

		RenderItem* RenderItem = nullptr;

		std::vector<std::pair<int, int> > LoadVertexBuffer(std::string MeshPath);
		void RegistMeshRenderList();

	public:
		MeshComponent(FPActor* Owner, std::string MeshPath);

		~MeshComponent();

		void SetMeshFill(bool State) { isFill = State; };
		void SetMeshCull(bool State) { isCull = State; };
		void SetActive(bool Active) { this->isActive = Active; }
		void SetTopology(Topology State) { Topo = State; };
		void SetPriority(int Prio) { Priority = Prio; };
		void SetMaterial(FPMaterial* Material) { Material = Material; };
};