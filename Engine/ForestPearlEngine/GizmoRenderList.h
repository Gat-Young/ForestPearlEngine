#pragma once
#include <vector>
#include "../ForestPearlEngine/Define/FPMath.h"

struct  GizmoRenderItem
{
	bool* Active;
	int* VBIndex;
	int* LineCount;
	FPVector3* Position;
	FPVector3* Rotation;
	FPVector3* Scale;
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
