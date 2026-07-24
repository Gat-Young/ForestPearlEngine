#pragma once
#include "Renderers/FPRenderingCommon.h"


class MeshRenderList
{
	private:
		std::vector<RenderItem> RenderList;

		MeshRenderList() = default;
		~MeshRenderList() = default;

	public:
		//Single Tone
		static MeshRenderList& Get()
		{
			static MeshRenderList Instance;
			return Instance;
		}

		RenderItem* RegistRenderList();
		void UnregistRenderList(RenderItem* RenderItem);

		std::vector<RenderItem>& GetRenderList()
		{
			return RenderList;
		}

};