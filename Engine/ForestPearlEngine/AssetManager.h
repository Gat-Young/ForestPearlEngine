#pragma once
#include <string>
#include <unordered_map>

struct FPMesh
{
	float x;
	float y;
	float z;
	float w;
	unsigned long color;
};


class Renderer;
struct COLVTX;

class AssetManager
{
	private:
		Renderer* FPRenderer;
		std::unordered_map<std::string, int> MeshMap;

		//Dummy Mesh <- 추후에 로드된 Mesh Data 사용
		std::unordered_map<std::string, std::vector<FPMesh> > DummyMesh;

		AssetManager();
		~AssetManager() = default;

		std::vector<COLVTX> ChangeCOLVTX(std::vector<FPMesh> Mesh);

	public:
		//Single Tone
		static AssetManager& Get()
		{
			static AssetManager Instance;
			return Instance;
		}

		int LordVertexVuffer(std::string MeshPath);
		void SetRenderer(Renderer* Renderer);
};