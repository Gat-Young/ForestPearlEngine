#pragma once
#include "MeshRenderList.h"
#include <string>

class MeshComponent
{
	private:
		std::string MeshData;
		int VBIndex;
		MeshRenderItem* RenderItem = nullptr;

		int LoadVertexBuffer();
		void RegistMeshRenderList();

	public:
		MeshComponent(std::string MeshPath);

		~MeshComponent();
};