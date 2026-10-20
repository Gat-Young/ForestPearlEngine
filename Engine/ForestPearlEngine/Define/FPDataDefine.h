#pragma once
#include <vector>
#include <string>


enum AssetOwner
{
	Engine,
	User
};

//정점 구조체 분리 
//Position
struct POSITION_VERTEX
{
	float x, y, z;		//좌표 Position
};

//Color
struct COLOR_VERTEX
{
	float r, g, b, a;	//Vertex 색상
};

//Normal
struct NORMAL_VERTEX
{
	float nx, ny, nz;	//노말 Normal
};

//UV
struct UV_VERTEX
{
	float u, v;			//좌표 UV
};

//Index
struct INDEX_16BIT
{
	unsigned short A, B, C;
};

struct INDEX_32BIT
{
	unsigned long A, B, C;
};

//VERTEX
struct VERTEX
{
	POSITION_VERTEX PositionVertex;
	NORMAL_VERTEX NormalVertex;
	COLOR_VERTEX ColorVertex;
	UV_VERTEX UVVertex;
	INDEX_32BIT Index;
};

//메시 데이터 구조체
struct FPMeshData
{
	std::string Name;
	std::vector<POSITION_VERTEX> Position_Vertices;
	std::vector<NORMAL_VERTEX> Normal_Vertices;
	std::vector<COLOR_VERTEX> Color_Vertices;
	std::vector<UV_VERTEX> UV_Vertices;
	std::vector<INDEX_32BIT> Index;
};

//소켓용 트랜스폼
struct SOCKET_TRANSFORM
{
	std::string SocketName;

	float Location_x, Location_y, Location_z;
	float Rotation_x, Rotation_y, Rotation_z;
	float Scale_x, Scale_y, Scale_Z;
};

//Static 메시 데이터 구조체
struct FPStaticMeshData
{
	std::string MeshPath;
	std::string MeshTopology;
	std::vector<SOCKET_TRANSFORM> Sockets;
	std::string MaterialName;
	AssetOwner Owner;
};

//Textue 데이터 구조체
struct FPTextureData
{
	void* TextureResource = nullptr;
	void* TextureDESC = nullptr;
};

//Texture Info
struct FPTextureInfo
{
	std::string Name;
	std::string Path;
	std::string Type;
	AssetOwner Owner;
};

//Material 데이터 구조체
struct FPMaterialData
{
	std::string Name;
	std::string VertexShader;
	std::string PixelShader;

	COLOR_VERTEX BaseColor;
	COLOR_VERTEX Ambient;
	COLOR_VERTEX Diffuse;

	std::vector<std::pair<int, std::string> > Textures;
};

//VertexBuffer Topology
enum class Topology
{
	TRIANGLELIST,
	TRIANGLESTRIP,
	LINELIST
};

Topology StringToTopology(std::string Topo);

//VertexBuffer 데이터 구조체
struct FPVertexBufferData
{
	void* VertexBuffer;
	unsigned long VB_Size;
	unsigned long VB_Cnt;
	unsigned int Stride;
	unsigned int Offset;
};

//IndexBuffer 데이터 구조체
struct FPIndexBufferData
{
	void* IndexBuffer;
	unsigned long Index_Size;
	unsigned long Index_Cnt;
	unsigned long FaceCnt;
	unsigned int Stride;
	unsigned int Offset;
};

struct FPActorData
{
	std::string ClassName;
	std::string ActorName;
	float Location_x, Location_y, Location_z;
	float Rotation_x, Rotation_y, Rotation_z;
	float Scale_x, Scale_y, Scale_Z;
};