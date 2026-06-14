#pragma once
#include "MeshRenderList.h"
#include <string>

class MeshComponent
{
	private:
		std::string MeshData;

		int VBIndex;
		bool isFill = true;
		bool isCull = true;

		MeshRenderItem* RenderItem = nullptr;

		int LoadVertexBuffer();
		void RegistMeshRenderList();

	public:
		MeshComponent(std::string MeshPath);

		~MeshComponent();

		void SetMeshFill(bool State) { isFill = State; };
		void SetMeshCull(bool State) { isCull = State; };
};