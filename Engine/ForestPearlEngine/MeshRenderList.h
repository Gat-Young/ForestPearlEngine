#pragma once
#include <vector>

struct Position
{
	float x, y, z;
};

struct Rotation
{
	float x, y, z;
};

struct Scale
{
	float x, y, z;
};

struct Transform
{
	Position position;
	Rotation rotation;
	Scale scale;
	Transform* Parent = nullptr;
};

struct MeshRenderItem
{
	int* VBIndex = nullptr;
	int* FaceSize = nullptr;
	bool* isFill = nullptr;
	bool* isCull = nullptr;
	Transform* transform = nullptr;
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