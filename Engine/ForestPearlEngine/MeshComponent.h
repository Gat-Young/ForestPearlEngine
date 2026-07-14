#pragma once
#include "MeshRenderList.h"
#include "FPPrimitiveComponent.h"
#include "FPMaterial.h"
#include <string>

class MeshComponent : FPPrimitiveComponent
{
	private:
		std::string MeshData;

		int VBIndex;
		int FaceSize;
		bool isFill = true;
		bool isCull = true;

		FPMaterial* Material = nullptr;

		MeshRenderItem* RenderItem = nullptr;

		std::pair<int, int> LoadVertexBuffer();
		void RegistMeshRenderList();

	public:
		MeshComponent(FPActor* Owner, std::string MeshPath);

		~MeshComponent();

		void SetMeshFill(bool State) { isFill = State; };
		void SetMeshCull(bool State) { isCull = State; };

		void SetMaterial(FPMaterial* Material) { Material = Material; };
};