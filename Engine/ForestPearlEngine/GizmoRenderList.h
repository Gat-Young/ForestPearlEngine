#pragma once
#include <vector>
#include "../ForestPearlEngine/Define/FPMath.h"

struct  GizmoRenderItem
{
	bool* Active = nullptr;
	int* VBIndex = nullptr;
	int* VertexSize = nullptr;
	FPVector3* Location = nullptr;
	FPQuaternion* Rotation = nullptr;
	FPVector3* Scale = nullptr;
	void* VertexShader = nullptr;
	void* PixelShader = nullptr;
	void* VBLayout = nullptr;
};

class GizmoRenderList
{
	private:
		std::vector<GizmoRenderItem> RenderList;

		GizmoRenderList() = default;
		~GizmoRenderList() = default;

	public:
		//Single Tone
		static GizmoRenderList& Get()
		{
			static GizmoRenderList Instance;
			return Instance;
		}

		GizmoRenderItem* RegistRenderList();
		void UnregistRenderList(GizmoRenderItem* RenderItem);

		std::vector<GizmoRenderItem>& GetRenderList()
		{
			return RenderList;
		}
};
