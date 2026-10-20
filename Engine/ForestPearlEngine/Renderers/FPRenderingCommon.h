#pragma once
#include "../Define/FPMath.h"
#include "FPConstantBufferCommon.h"
#include <string>
#include "tchar.h"
#include <vector>
#include <unordered_map>
#include <tuple>

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


//VertexBuffer Topology
enum class Topology
{
	TRIANGLELIST,
	TRIANGLESTRIP,
	LINELIST
};

Topology StringToTopology(std::string Topo);

//Static 메시 데이터 구조체
struct FPStaticMeshData
{
	std::string Name;
	std::string NodeName;
	std::string MeshPath;
	Topology MeshTopology;
	std::vector<SOCKET_TRANSFORM> Sockets;
	std::string MaterialName;
	AssetOwner Owner;
};


enum FPSamplerType
{
	Sampler_CLAMP,
	Sampler_Wrap,
	Sampler_Mirror,
	Sampler_Mirror_Once,
	Sampler_BorderColor_RED,
	Sampler_BorderColor_BLUE,
	Sampler_BorderColor_GREEN,
	Sampler_BorderColor_MAGENTA,

	Sampler_MAX_
};

FPSamplerType StringToSamplerType(const std::string& Name);

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
	AssetOwner Owner;

	COLOR_VERTEX BaseColor;
	COLOR_VERTEX Ambient;
	COLOR_VERTEX Diffuse;

	//<Slot, Name, Samplertypr>
	std::vector<std::tuple<unsigned int, std::string, FPSamplerType> > Textures;
};

//VertexShader 데이터 구조체
struct FPVertexShaderData
{
	std::string Name;
	std::string Path;
	std::string InputLayout;
	AssetOwner Owner;
};

//PixelShader 데이터 구조체
struct FPPixelShaderData
{
	std::string Name;
	std::string Path;
	AssetOwner Owner;
};

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

struct FPTextureItem
{
	std::string* TexturePath;
	FPSamplerType* SamplerType;
};

struct RenderItem
{
	int* Priority = nullptr;
	bool* Active = nullptr;
	std::string* MeshPath = nullptr;

	//핵심 데이터
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;

	FPVector4* BaseColor = nullptr;
	FPVector4* Diffuse = nullptr;
	FPVector4* Ambient = nullptr;

	std::unordered_map<int, FPTextureItem>* TextureList;

	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};

struct GizmoRenderItem
{
	int* Priority = nullptr;
	bool* Active = nullptr;

	std::string* MeshPath = nullptr;

	//핵심 데이터
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;


	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;

	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};

struct NormalRenderItem
{
	bool* Active = nullptr;
	std::string* MeshPath = nullptr;
	float* NormalLength = nullptr;

	//핵심 데이터
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;
};

struct CameraItem
{
	//카메라의 위치
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	FPMatrix* View = nullptr;
	FPMatrix* Projection = nullptr;

	bool* Active = nullptr;			//카메라 사용 여부
	bool* TripleCam = nullptr;
};

struct UIContextItem
{
	int* Priority;
	bool** active;
	int* x;
	int* y;
	FPVector4* color;
	std::basic_string<TCHAR>* msg;
};

struct FPViewPort
{
	float TopLeftX = 0.0f;
	float TopLeftY = 0.0f;
	float Width;
	float Height;
	float MinDepth = 0.0f;
	float MaxDepth = 1.0f;
	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};

struct FPLightRenderItem
{
	bool* Active = nullptr;

	FPVector3* Direction = nullptr;
	float* Range = nullptr;
	FPVector4* Diffuse = nullptr;
	FPVector4* Ambient = nullptr;

	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	//상수 버퍼용
	FPConstantBufferRenderData* VertexConstBuffer = nullptr;
	FPConstantBufferRenderData* PixelConstBuffer = nullptr;
};

namespace RenderingData
{

	struct GizmoRenderItem
	{
		int Priority;

		std::string MeshPath;

		//주요 정보
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct alignas(16) NormalDebugItem
	{
		FPMatrix NormalMatrix;
		float NormalLength = 1.0f;
		float Padding[3] = {};
	};

	struct NormalRenderItem
	{
		std::string MeshPath;
		float NormalLength;

		//핵심 데이터
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;


		//상수 버퍼용
		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct FPViewPort
	{
		float TopLeftX = 0.0f;
		float TopLeftY = 0.0f;
		float Width;
		float Height;
		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct CameraItem
	{
		//카메라의 위치
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		FPMatrix View;
		FPMatrix Projection;

		std::vector<FPViewPort> ViewPort;
	};

	struct UIContextItem
	{
		int Priority;
		bool active;
		int x;
		int y;
		FPVector4 color;
		std::basic_string<TCHAR> msg;

		std::vector<FPViewPort> ViewPort;
	};

	struct alignas(16) DirectionLightItem
	{
		FPVector4 Direction;
		float Range;
		float padding[3];
		FPVector4 Diffuse;
		FPVector4 Ambient;
	};

	struct alignas(16) MaterialItem
	{
		FPVector4 BaseColor;
		FPVector4 Diffuse;
		FPVector4 Ambient;
	};

	//상수 버퍼용 구조체 : 셰이더 내부 연산에 사용될 데이터들
	struct alignas(16) MVPConstBuffer
	{
		DirectX::XMMATRIX WorldMatrix;
		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX ProjMatrix;
		DirectX::XMMATRIX WVMatrix;
		DirectX::XMMATRIX WVPMatrix;
	};

	struct LightRenderItem
	{
		FPVector3 Direction;
		float Range;
		FPVector4 Diffuse;
		FPVector4 Ambient;

		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		//상수 버퍼용
		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct FPTextureItem
	{
		int Slot;
		std::string TexturePath;
		FPSamplerType SamplerType;
	};

	struct FPSamplerItem
	{
		int Slot;
		FPSamplerType SamplerType;
	};

	struct MeshRenderItem
	{
		int Priority;

		std::string MeshPath;

		//주요 정보
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;

		std::vector<RenderingData::FPTextureItem> TextureList;
		std::vector<RenderingData::FPSamplerItem> SamplerList;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	struct MeshRenderItemCompare
	{
		bool operator() (const RenderingData::MeshRenderItem& Left, const RenderingData::MeshRenderItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};

	struct GizmoRenderItemCompare
	{
		bool operator() (const RenderingData::GizmoRenderItem& Left, const RenderingData::GizmoRenderItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};

	struct UIRenderItemCompare
	{
		bool operator() (const RenderingData::UIContextItem& Left, const RenderingData::UIContextItem& Right) const
		{
			return Left.Priority < Right.Priority;
		};
	};
}
