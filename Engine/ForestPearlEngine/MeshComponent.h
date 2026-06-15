#pragma once
#include "MeshRenderList.h"
#include <string>

class MeshComponent
{
	private:
		std::string MeshData;

		int VBIndex;
		int FaceSize;
		bool isFill = true;
		bool isCull = true;

		MeshRenderItem* RenderItem = nullptr;

		std::pair<int, int> LoadVertexBuffer();
		void RegistMeshRenderList(Transform* transform);

	public:
		MeshComponent(std::string MeshPath, Transform* transform);

		~MeshComponent();

		void SetMeshFill(bool State) { isFill = State; };
		void SetMeshCull(bool State) { isCull = State; };
};