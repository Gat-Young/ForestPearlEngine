#include "AssetManager.h"
#include "Renderers/RenderingDevice.h"
#include "Libraries/Ufbx/ufbx.h"
#include <iostream>
//Á¤Á¡ ±¸Á¶Ã¼
struct VERTEX
{
	float x, y, z;		//ÁÂÇ¥ Position
	float r, g, b, a;	//»ö»ó Diffuse Color
};


AssetManager::AssetManager()
{
	ufbx_scene* Scene = ufbx_load_file("../../Engine/ForestPearlengine/Assets/Model/ToonLink/ToonLinkTriangle.fbx", nullptr, nullptr);
	for (ufbx_node* node : Scene->nodes)
	{
		std::cout << node->name.data << "\n";
	}
	ufbx_free_scene(Scene);
	//DummyMesh["Orb"].push_back({ -0.5f, -0.5f, 0.0f, 0xff00f0ff });
	//DummyMesh["Orb"].push_back({ 0.0f, 0.5f, 0.0f,  0xff44ffae });
	//DummyMesh["Orb"].push_back({ 0.5f, -0.5f, 0.0f,  0xfffffd71 });

	////»ï°¢ÇüÀ» À§ÇÑ 3°³ÀÇ Á¤Á¡ ¼±¾ð. : Á¤Á¡ÀÇ ÁÂÇ¥°ªÀº È­¸é(Screen)ÁÂÇ¥.
	////Face 0 : Á¤»ï°¢Çü.(CW)
	//// ÁÂÇ¥ (x, y)    »ö»ó( a, r, g, b)   a, °ð Alpha ´Â ±âº»°ª 255 (1.0f)
#define ChangeRGBA(col) ((col & 0x00ff0000) >> 16) /255, ((col & 0x0000ff00) >> 8) /255,((col & 0x000000ff)) /255,  ((col & 0xff000000)>>24) /255
	DummyMesh["Triangle"].push_back({ -0.5f, 0.0f, 0.0f, ChangeRGBA(0xffff0000) });			//v0, Red.	¡Ú
	DummyMesh["Triangle"].push_back({ 0.0f, 1.0f, 0.0f,  ChangeRGBA(0xff00ff00) });		//v1, Green ¡Ú
	DummyMesh["Triangle"].push_back({ 0.5f, 0.0f, 0.0f,  ChangeRGBA(0xff0000ff) });		//v2, Blue ¡Ú

	////Face 1 : ¿ª»ï°¢Çü.(CCW)
	DummyMesh["Triangle"].push_back({ 50.0f, 250.0f, 0.5f,  ChangeRGBA(0xffff0000) });
	DummyMesh["Triangle"].push_back({ 150.0f, 450.0f, 0.5f, ChangeRGBA(0xff00ff00) });
	DummyMesh["Triangle"].push_back({ 250.0f, 250.0f, 0.5f, ChangeRGBA(0xff0000ff) });

	////Face 2: ºø°¢ »ï°¢Çü (CW) Å×½ºÆ® »ï°¢Çü
	DummyMesh["Triangle"].push_back({ 300.0f, 500.0f, 0.5f, ChangeRGBA(0xffff0000) });
	DummyMesh["Triangle"].push_back({ 400.0f, 300.0f, 0.5f, ChangeRGBA(0xff00ff00) });
	DummyMesh["Triangle"].push_back({ 480.0f, 430.0f, 0.5f, ChangeRGBA(0xff0000ff) });

	////Face 3: ºø°¢ »ï°¢Çü (CCW) Å×½ºÆ® »ï°¢Çü
	DummyMesh["Triangle"].push_back({ 500.0f, 430.0f, 0.5f, ChangeRGBA(0xffff0000) });
	DummyMesh["Triangle"].push_back({ 680.0f, 500.0f, 0.5f, ChangeRGBA(0xff00ff00) });
	DummyMesh["Triangle"].push_back({ 600.0f, 300.0f, 0.5f, ChangeRGBA(0xff0000ff) });

	////Face 4 : Á÷°¢ »ï°¢2 (CW)
	DummyMesh["Triangle"].push_back({ 40.0f,  30.0f, 0.5f, ChangeRGBA(0xffff0000) });
	DummyMesh["Triangle"].push_back({ 90.0f,  30.0f, 0.5f, ChangeRGBA(0xff00ff00) });
	DummyMesh["Triangle"].push_back({ 90.0f, 100.0f, 0.5f, ChangeRGBA(0xff0000ff) });

	////Face 5 : Á÷°¢ »ï°¢1 (CCW)
	DummyMesh["Triangle"].push_back({ 10.0f,  30.0f, 0.5f, ChangeRGBA(0xffff0000) });
	DummyMesh["Triangle"].push_back({ 10.0f, 100.0f, 0.5f, ChangeRGBA(0xff00ff00) });
	DummyMesh["Triangle"].push_back({ 60.0f, 100.0f, 0.5f, ChangeRGBA(0xff0000ff) });


	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////

	////Face 0 : Á¤»ï°¢Çü.(CW)
	//// 3D ÁÂÇ¥ (x, y, z)   »ö»ó( a, r, g, b)   a, °ð Alpha ´Â ±âº»°ª 255 (1.0f)
	DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 0.0f, ChangeRGBA(0xffff0000) });			//v0, Red.	¡Ú
	DummyMesh["Triangle2"].push_back({ 0.0f, 1.0f, 0.0f,  ChangeRGBA(0xff00ff00) });		//v1, Green ¡Ú
	DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 0.0f,  ChangeRGBA(0xff0000ff) });		//v2, Blue ¡Ú

	////Face 1 : ¿ª»ï°¢Çü.(CCW)
	DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 0.0f, ChangeRGBA(0xffff0000) });
	DummyMesh["Triangle2"].push_back({ 0.0f,-1.0f, 0.0f, ChangeRGBA(0xff00ff00) });
	DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 0.0f, ChangeRGBA(0xff0000ff) });

	//// ¸ðµ¨ µ¥ÀÌÅÍ Z=5.0 ¿¡ ÁÖÀÇ.
	//	// ÁÂÇ¥ (x, y, z)    »ö»ó( a, r, g, b)   a, °ð Alpha ´Â ±âº»°ª 255 (1.0f)
	//DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 5.0f, ChangeRGBA(0xffff0000) });			//v0, Red.	¡Ú
	//DummyMesh["Triangle2"].push_back({ 0.0f, 1.0f, 5.0f,  ChangeRGBA(0xff00ff00) });		//v1, Green ¡Ú
	//DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 5.0f,  ChangeRGBA(0xff0000ff) });		//v2, Blue ¡Ú

	////Face 1 : ¿ª»ï°¢Çü.(CCW)
	//DummyMesh["Triangle2"].push_back({ -0.5f, 0.0f, 5.0f, ChangeRGBA(0xffff0000) });
	//DummyMesh["Triangle2"].push_back({ 0.0f,-1.0f, 5.0f, ChangeRGBA(0xff00ff00) });
	//DummyMesh["Triangle2"].push_back({ 0.5f, 0.0f, 5.0f, ChangeRGBA(0xff0000ff) });
	

	//Face 0 : Á¤»ï°¢Çü.(CW) 
	// 3D ÁÂÇ¥ (x, y, z)   »ö»ó( a, r, g, b)   a, °ð Alpha ´Â ±âº»°ª 255 (1.0f) 
	DummyMesh["Triangle3"].push_back({ -0.5f, 0.0f, 0.0f, 1, 0, 0, 1 });			//v0, Red.	¡Ú
	DummyMesh["Triangle3"].push_back({ 0.0f, 1.0f, 0.0f,  0, 1, 0, 1 });		//v1, Green ¡Ú
	DummyMesh["Triangle3"].push_back({ 0.5f, 0.0f, 0.0f,  0, 0, 1, 1 });		//v2, Blue ¡Ú

	//Face 0 : »ï°¢Çü.(CW) 
	DummyMesh["Triangle4"].push_back({ -10.0f, 0.0f, 0.0f, 1, 0, 0, 1 });			//v0, Red.	¡Ú
	DummyMesh["Triangle4"].push_back({ 0.0f, 12.0f, 0.0f,  0, 1, 0, 1 });		//v1, Green ¡Ú
	DummyMesh["Triangle4"].push_back({ 10.0f, 0.0f, 0.0f,  0, 0, 1, 1 });		//v2, Blue ¡Ú
}

std::pair<int, int> AssetManager::LordVertexBuffer(std::string MeshPath)
{
	if (MeshMap.count(MeshPath) > 0)
	{
		return { MeshMap[MeshPath], DummyMesh[MeshPath].size() };
	}

	//ÃßÈÄ ¸Þ½Ã ÆÄÀÏ ·Îµå·Î º¯°æ
	std::vector<FPMesh> LoadMesh = DummyMesh[MeshPath];

	MeshMap[MeshPath] = RenderingDevice::GetRenderingDevice().CreateVertexBuffer(ChangeVERTEX(LoadMesh).data(), LoadMesh.size(), sizeof(VERTEX));

	return {MeshMap[MeshPath], LoadMesh.size()};
}

int AssetManager::MakeVertexBuffer(std::vector<FPMesh> Mesh)
{
	return RenderingDevice::GetRenderingDevice().CreateVertexBuffer(ChangeVERTEX(Mesh).data(), Mesh.size(), sizeof(VERTEX));
}

std::vector<VERTEX> AssetManager::ChangeVERTEX(std::vector<FPMesh> Mesh)
{
	std::vector<VERTEX> Vertex;

	for (int i = 0; i < Mesh.size(); ++i)
	{
		Vertex.push_back(VERTEX{ Mesh[i].vPos.x, Mesh[i].vPos.y, Mesh[i].vPos.z, Mesh[i].r, Mesh[i].g, Mesh[i].b, Mesh[i].a});
	}

	return Vertex;
}
