#pragma once
#include <vector>
#include "Define/FPMath.h"

struct MeshRenderItem
{
	int* VBIndex = nullptr;
	int* FaceSize = nullptr;
	bool* isFill = nullptr;
	bool* isCull = nullptr;
	FPVector3* Location = nullptr;
	FPQuaternion* Rotation = nullptr;
	FPVector3* Scale = nullptr;
	void* VertexShader = nullptr;
	void* PixelShader = nullptr;
};

class MeshRenderList
{
	private:
		std::vector<MeshRenderItem> RenderList;

		MeshRenderList() = default;
		~MeshRenderList() = default;

	public:
		//Single Tone
		static MeshRenderList& Get()
		{
			static MeshRenderList Instance;
			return Instance;
		}

		MeshRenderItem* RegistRenderList();
		void UnregistRenderList(MeshRenderItem* RenderItem);

		std::vector<MeshRenderItem>& GetRenderList()
		{
			return RenderList;
		}

};