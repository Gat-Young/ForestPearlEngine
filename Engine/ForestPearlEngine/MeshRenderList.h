#pragma once
#include <vector>

struct MeshRenderItem
{
	int* VBIndex = nullptr;
	bool* isFill = nullptr;
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