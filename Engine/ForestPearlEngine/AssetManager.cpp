#include "AssetManager.h"
#include "Renderers/Renderer.h"


AssetManager::AssetManager()
{
	//삼각형을 위한 3개의 정점 선언. : 정점의 좌표값은 화면(Screen)좌표.
	//Face 0 : 정삼각형.(CW) 
	// 좌표 (x, y)    색상( a, r, g, b)   a, 곧 Alpha 는 기본값 255 (1.0f)
	DummyMesh["Triangle"].push_back({ -0.5f, 0.0f, 0.0f, 0xffff0000 });			//v0, Red.	★
	DummyMesh["Triangle"].push_back({ 0.0f, 1.0f, 0.0f,  0xff00ff00 });		//v1, Green ★
	DummyMesh["Triangle"].push_back({ 0.5f, 0.0f, 0.0f,  0xff0000ff });		//v2, Blue ★

	//Face 1 : 역삼각형.(CCW) 
	DummyMesh["Triangle"].push_back({ 50.0f, 250.0f, 0.5f,  0xffff0000 });
	DummyMesh["Triangle"].push_back({ 150.0f, 450.0f, 0.5f, 0xff00ff00 });
	DummyMesh["Triangle"].push_back({ 250.0f, 250.0f, 0.5f, 0xff0000ff });

	//Face 2: 빗각 삼각형 (CW) 테스트 삼각형 
	DummyMesh["Triangle"].push_back({ 300.0f, 500.0f, 0.5f, 0xffff0000 });
	DummyMesh["Triangle"].push_back({ 400.0f, 300.0f, 0.5f, 0xff00ff00 });
	DummyMesh["Triangle"].push_back({ 480.0f, 430.0f, 0.5f, 0xff0000ff });

	//Face 3: 빗각 삼각형 (CCW) 테스트 삼각형 
	DummyMesh["Triangle"].push_back({ 500.0f, 430.0f, 0.5f, 0xffff0000 });
	DummyMesh["Triangle"].push_back({ 680.0f, 500.0f, 0.5f, 0xff00ff00 });
	DummyMesh["Triangle"].push_back({ 600.0f, 300.0f, 0.5f, 0xff0000ff });

	//Face 4 : 직각 삼각2 (CW)
	DummyMesh["Triangle"].push_back({ 40.0f,  30.0f, 0.5f, 0xffff0000 });
	DummyMesh["Triangle"].push_back({ 90.0f,  30.0f, 0.5f, 0xff00ff00 });
	DummyMesh["Triangle"].push_back({ 90.0f, 100.0f, 0.5f, 0xff0000ff });

	//Face 5 : 직각 삼각1 (CCW)
	DummyMesh["Triangle"].push_back({ 10.0f,  30.0f, 0.5f, 0xffff0000 });
	DummyMesh["Triangle"].push_back({ 10.0f, 100.0f, 0.5f, 0xff00ff00 });
	DummyMesh["Triangle"].push_back({ 60.0f, 100.0f, 0.5f, 0xff0000ff });


	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////

	//Face 0 : 정삼각형.(CW) 
	// 3D 좌표 (x, y, z)   색상( a, r, g, b)   a, 곧 Alpha 는 기본값 255 (1.0f) 
	DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 0.0f, 0xffff0000 });			//v0, Red.	★
	DummyMesh["Triangle2"].push_back({ 0.0f, 1.0f, 0.0f,  0xff00ff00 });		//v1, Green ★
	DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 0.0f,  0xff0000ff });		//v2, Blue ★

	//Face 1 : 역삼각형.(CCW) 
	DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 0.0f, 0xffff0000 });
	DummyMesh["Triangle2"].push_back({ 0.0f,-1.0f, 0.0f, 0xff00ff00 });
	DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 0.0f, 0xff0000ff });

	/*// 모델 데이터 Z=5.0 에 주의.
		// 좌표 (x, y, z)    색상( a, r, g, b)   a, 곧 Alpha 는 기본값 255 (1.0f)
	DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 5.0f, 1.0f,  0xffff0000 });			//v0, Red.	★
	DummyMesh["Triangle2"].push_back({ 0.0f, 1.0f, 5.0f,  1.0f,  0xff00ff00 });		//v1, Green ★
	DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 5.0f,  1.0f,	 0xff0000ff });		//v2, Blue ★

	//Face 1 : 역삼각형.(CCW) 
	DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 5.0f, 1.0f, 0xffff0000 });
	DummyMesh["Triangle2"].push_back({ 0.0f,-1.0f, 5.0f, 1.0f, 0xff00ff00 });
	DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 5.0f, 1.0f, 0xff0000ff });
	*/

	//Face 0 : 정삼각형.(CW) 
	// 3D 좌표 (x, y, z)   색상( a, r, g, b)   a, 곧 Alpha 는 기본값 255 (1.0f) 
	DummyMesh["Triangle3"].push_back({ -0.5f, 0.0f, 0.0f, 0xffff0000 });			//v0, Red.	★
	DummyMesh["Triangle3"].push_back({ 0.0f, 1.0f, 0.0f,  0xff00ff00 });		//v1, Green ★
	DummyMesh["Triangle3"].push_back({ 0.5f, 0.0f, 0.0f,  0xff0000ff });		//v2, Blue ★
}

void AssetManager::SetRenderer(Renderer* Renderer)
{
	FPRenderer = Renderer;
}


std::pair<int, int> AssetManager::LordVertexVuffer(std::string MeshPath)
{
	if (MeshMap.count(MeshPath) > 0)
	{
		return { MeshMap[MeshPath], DummyMesh[MeshPath].size() };
	}

	//추후 메시 파일 로드로 변경
	std::vector<FPMesh> LoadMesh = DummyMesh[MeshPath];

	MeshMap[MeshPath] = FPRenderer->MakeVB(ChangeCOLVTX(LoadMesh));

	return {MeshMap[MeshPath], LoadMesh.size()/3};
}

int AssetManager::MakeVertexVuffer(std::vector<FPMesh> Mesh)
{
	return FPRenderer->MakeVB(ChangeCOLVTX(Mesh));
}

std::vector<COLVTX> AssetManager::ChangeCOLVTX(std::vector<FPMesh> Mesh)
{
	std::vector<COLVTX> Vertex;

	for (int i = 0; i < Mesh.size(); ++i)
	{
		Vertex.push_back(MakeCOLVTX(Mesh[i].vPos.x, Mesh[i].vPos.y, Mesh[i].vPos.z, Mesh[i].color));
	}

	return Vertex;
}
