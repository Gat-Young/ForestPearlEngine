#pragma once
#include "../Define/FPMath.h"
#include "FPConstantBufferCommon.h"
#include <string>
#include "tchar.h"
#include <vector>
#include <unordered_map>
#include <tuple>

//Project Setting
struct FPGameSettingData
{
	std::string GameName;

	// Graphics
	int MSAA = 1;
	int AnisotropicFiltering = 1;

	int Width = 960;
	int Height = 600;

	bool FullScreen = false;
};

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
	float r = 0, g = 0, b = 0, a = 0;	//Vertex 색상
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
	std::string NodeName;
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
	//Point
	Filter_Point_Sampler_CLAMP,
	Filter_Point_Sampler_Wrap,
	Filter_Point_Sampler_Mirror,
	Filter_Point_Sampler_Mirror_Once,
	Filter_Point_Sampler_Border,
	Filter_Point_Sampler_BorderColor_RED,
	Filter_Point_Sampler_BorderColor_BLUE,
	Filter_Point_Sampler_BorderColor_GREEN,
	Filter_Point_Sampler_BorderColor_MAGENTA,

	//Bi-Linear
	Filter_Bi_Linear_Sampler_CLAMP,
	Filter_Bi_Linear_Sampler_Wrap,
	Filter_Bi_Linear_Sampler_Mirror,
	Filter_Bi_Linear_Sampler_Mirror_Once,
	Filter_Bi_Linear_Sampler_Border,
	Filter_Bi_Linear_Sampler_BorderColor_RED,
	Filter_Bi_Linear_Sampler_BorderColor_BLUE,
	Filter_Bi_Linear_Sampler_BorderColor_GREEN,
	Filter_Bi_Linear_Sampler_BorderColor_MAGENTA,

	//Tri-Linear (Mip-Map On)
	Filter_Tri_Linear_Sampler_CLAMP,
	Filter_Tri_Linear_Sampler_Wrap,
	Filter_Tri_Linear_Sampler_Mirror,
	Filter_Tri_Linear_Sampler_Mirror_Once,
	Filter_Tri_Linear_Sampler_Border,
	Filter_Tri_Linear_Sampler_BorderColor_RED,
	Filter_Tri_Linear_Sampler_BorderColor_BLUE,
	Filter_Tri_Linear_Sampler_BorderColor_GREEN,
	Filter_Tri_Linear_Sampler_BorderColor_MAGENTA,

	//Anistropy
	Filter_Anistropy_Linear_Sampler_CLAMP,
	Filter_Anistropy_Linear_Sampler_Wrap,
	Filter_Anistropy_Linear_Sampler_Mirror,
	Filter_Anistropy_Linear_Sampler_Mirror_Once,
	Filter_Anistropy_Linear_Sampler_Border,
	Filter_Anistropy_Linear_Sampler_BorderColor_RED,
	Filter_Anistropy_Linear_Sampler_BorderColor_BLUE,
	Filter_Anistropy_Linear_Sampler_BorderColor_GREEN,
	Filter_Anistropy_Linear_Sampler_BorderColor_MAGENTA,

	Sampler_MAX_
};

FPSamplerType StringToSamplerType(const std::string& Name);

enum FPBlendingStateType
{
	BS_DEFAULT,	// 기본 혼합 객체
	BS_ADD,		// 덧셈

	BS_NULL_,	//초기화
	BS_MAX_
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

//Texture Array Info
struct FPTextureArrayInfo
{
	std::string Name;
	unsigned int Width;
	unsigned int Height;
	unsigned int ArraySize;

	std::vector<std::string> SourceTextures;
	AssetOwner Owner;
};

enum FPTextureType
{
	Texture2D,
	Texture2DArray
};

FPTextureType StringToTextureType(const std::string& Name);

struct FPMaterial_TextureData
{
	unsigned int Slot;
	std::string Name;
	FPTextureType TextureType;
	FPSamplerType SamplerType;
};

//Material 데이터 구조체
struct FPMaterialData
{
	std::string Name;
	std::string VertexShader;
	std::string PixelShader;
	AssetOwner Owner;

	bool TwoSided;

	COLOR_VERTEX BaseColor;
	COLOR_VERTEX Ambient;
	COLOR_VERTEX Diffuse;
	COLOR_VERTEX Emissive;

	//<Slot, Name, Samplertypr>
	std::vector<FPMaterial_TextureData> Textures;
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
	FPTextureType* TextureType;
};

struct RenderItem
{
	int* Priority = nullptr;
	bool* Active = nullptr;

	std::string* MeshPath = nullptr;
	
	//핵심 데이터
	bool* TwoSided = nullptr;

	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;

	FPVector4* BaseColor = nullptr;
	FPVector4* Diffuse = nullptr;
	FPVector4* Ambient = nullptr;
	FPVector4* Emissive = nullptr;

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

enum FPLightType
{
	LightType_Direction,
	LightType_Point,
	LightType_Spot,
	LightType_Rect,

	LightType_None,
};

struct FPLightRenderItem
{
	bool* Active = nullptr;

	FPVector3* Direction = nullptr;
	float* Range = nullptr;
	FPVector4* Diffuse = nullptr;
	FPVector4* Ambient = nullptr;
	FPVector4* Specular = nullptr;
	FPVector4* Attenuation = nullptr;
	FPVector4* LightColor = nullptr;
	unsigned int* LightType = nullptr;

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
		float NormalSign = 1.0f;
		float Padding[2] = {};
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

	constexpr unsigned int Light_MAX = 64;

	struct alignas(16) LightDataItem
	{
		unsigned int LightType;
		unsigned int padding[3];
		FPVector4 LightPosition;
		FPVector4 Direction;
		float Range;
		float padding_2[3];
		FPVector4 Diffuse;
		FPVector4 Ambient;
		FPVector4 Specular;
		FPVector4 Attenuation;
		FPVector4 LightColor;
	};

	struct alignas(16) LightItem
	{
		unsigned int LightCnt = 0;
		unsigned int Padding[3];
		LightDataItem Light[Light_MAX];
	};

	struct alignas(16) MaterialItem
	{
		FPVector4 BaseColor;
		FPVector4 Diffuse;
		FPVector4 Ambient;
		FPVector4 Emissive;
	};

	//상수 버퍼용 구조체 : 셰이더 내부 연산에 사용될 데이터들
	struct alignas(16) MVPConstBuffer
	{
		DirectX::XMMATRIX WorldMatrix;
		DirectX::XMMATRIX InverseWorldMatrix;
		DirectX::XMMATRIX TranseposeWorldMatrix;
		DirectX::XMMATRIX InverseTranseposeWorldMatrix;

		DirectX::XMMATRIX ViewMatrix;
		DirectX::XMMATRIX InverseViewMatrix;
		DirectX::XMMATRIX TranseposeViewMatrix;
		DirectX::XMMATRIX InverseTranseposeViewMatrix;

		DirectX::XMMATRIX ProjMatrix;
		DirectX::XMMATRIX InverseProjMatrix;
		DirectX::XMMATRIX TranseposeProjMatrix;
		DirectX::XMMATRIX InverseTranseposeProjMatrix;

		DirectX::XMMATRIX WVMatrix;
		DirectX::XMMATRIX InverseWVMatrix;
		DirectX::XMMATRIX TranseposeWVMatrix;
		DirectX::XMMATRIX InverseTranseposeWVMatrix;

		DirectX::XMMATRIX WVPMatrix;
		DirectX::XMMATRIX InverseWVPMatrix;
		DirectX::XMMATRIX TranseposeWVPMatrix;
		DirectX::XMMATRIX InverseTranseposeWVPMatrix;
	};

	struct LightRenderItem
	{
		FPVector3 Direction;
		float Range;
		FPVector4 Diffuse;
		FPVector4 Ambient;
		FPVector4 Specular;
		FPVector4 Attenuation;
		FPVector4 LightColor;
		unsigned int LightType;

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
		bool TwoSided;

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
