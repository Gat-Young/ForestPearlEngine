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

	// Display
	bool VSync = true;
	int Hz = 60;
};

enum AssetOwner
{
	Engine,
	User
};

//Camera / SceneCapture components Projection mode
enum FPProjectionMode
{
	Perspective,
	Orthographic
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

//Tangnet , w는 TangentSign : 방향 값, Binormal을 외적과 TangentSign으로 구함
struct TANGENT_VERTEX
{
	float x, y, z, w;
};

//VERTEX
struct VERTEX
{
	POSITION_VERTEX PositionVertex;
	NORMAL_VERTEX NormalVertex;
	COLOR_VERTEX ColorVertex;
	UV_VERTEX UVVertex;
	TANGENT_VERTEX Tangent;
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
	std::vector<TANGENT_VERTEX> Tangent_Vertices;
	std::vector<INDEX_32BIT> Index;
	std::vector<std::vector<uint32_t>> VertexAdjacency;

	FPAABB BoundingBox;
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
	int StencilRefValue = 0;
	bool bCustomDepthStencil = false;
	AssetOwner Owner;
};


enum class FPSamplerFilterType
{
	Point,
	BiLinear,
	TriLinear,
	Anisotropic,
	ComparisonBiLinear,

	FPSamplerFilterType_MAX_
};

FPSamplerFilterType StringToFPSamplerFilterType(const std::string& Name);

enum class FPTextureAddressMode
{
	CLAMP,
	WRAP,
	MIRROR,
	MIRROR_ONCE,
	BORDER,

	FPTextureAddressMode_MAX_
};

FPTextureAddressMode StringToFPTextureAddressMode(const std::string& Name);

enum class FPBlend
{
	ZERO,
	ONE,
	SRC_COLOR,
	INV_SRC_COLOR,
	SRC_ALPHA,
	INV_SRC_ALPHA,
	DEST_ALPHA,
	INV_DEST_ALPHA,
	DEST_COLOR,
	INV_DEST_COLOR,
	SRC_ALPHA_SAT,
	BLEND_FACTOR,
	INV_BLEND_FACTOR,
	SRC1_COLOR,
	INV_SRC1_COLOR,
	SRC1_ALPHA,
	INV_SRC1_ALPHA,

	FPBlend_MAX_
};

FPBlend StringToFPBlend(const std::string& Name);

enum class FPBlendOp
{
	ADD,
	SUBTRACT,
	REV_SUBTRACT,
	MIN,
	MAX,

	FPBlendOp_MAX_
};

FPBlendOp StringToFPBlendOp(const std::string& Name);

enum class FPColorWriteMask
{
	RED,
	GREEN,
	BLUE,
	ALPHA,

	FPColorWriteMask_MAX_
};

FPColorWriteMask StringToFPColorWriteMask(const std::string& Name);

//Textue 데이터 구조체
struct FPTextureData
{
	void* ShaderResourceView = nullptr;
	void* TextureDESC = nullptr;
};

struct FPTextureArrayData
{
	void* TextureArraySRV = nullptr;
};

//Texture Info
struct FPTextureInfo
{
	std::string Name;
	std::string Path;
	std::string Type;
	bool CubeMap = false;			//CubeMap Texture 생성 여부
	AssetOwner Owner;
};

//RenderTarget

enum class FPRenderTargetFormat
{
	R8,
	RG8,
	RGBA8,
	RGBA8_SRGB,
	R16F,
	RGBA16F,
	R32F,
	RGBA32F,
	RGBA10A2,


	FPRenderTargetType_MAX_
};

FPRenderTargetFormat StringToFPRenderTargetFormat(const std::string& Name);

struct FPRenderTargetTexture2DInfo
{
	std::string Name;
	std::string Type;
	unsigned int SizeX;
	unsigned int SizeY;
	float ScaleX;
	float ScaleY;
	FPRenderTargetFormat RenderTragetFormat = FPRenderTargetFormat::RGBA8;
	FPVector4 ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
	unsigned int MSAA = 1;
	bool MipMap = false;
	AssetOwner Owner;
};

struct FPRenderTargetData
{
	void* Texture = nullptr;
	void* ResolveTexture = nullptr;
	void* RenderTargetView = nullptr;
	void* ShaderResourceView = nullptr;
};

//DepthStencill

enum class FPDepthStencilFormat
{
	D16,
	D24_S8_UINT,
	D32F,
	D32F_S8X24_UINT,

	FPDepthStencilFormat_MAX_
};

FPDepthStencilFormat StringToFPDepthStencilFormat(const std::string& Name);

struct FPDepthStencilInfo
{
	std::string Name;
	unsigned int SizeX;
	unsigned int SizeY;
	float ScaleX;
	float ScaleY;
	FPDepthStencilFormat DepthStencilFormat = FPDepthStencilFormat::D24_S8_UINT;
	float ClearDepth = 1.0;
	unsigned int ClearStencil = 0;
	unsigned int MSAA = 1;
	bool ShaderResource = false;


	AssetOwner Owner;
};

struct FPDepthStencilData
{
	void* Texture = nullptr;
	void* DepthStencilView = nullptr;
	void* ShaderResourceView = nullptr;
	void* StencilShaderResourceView = nullptr;
};

//DepthStencil Array
struct FPDepthStencilArrayInfo
{
	std::string Name;
	unsigned int SizeX;
	unsigned int SizeY;
	float ScaleX;
	float ScaleY;
	FPDepthStencilFormat DepthStencilFormat = FPDepthStencilFormat::D24_S8_UINT;
	int ArraySize = 1;
	float ClearDepth = 1.0;
	unsigned int ClearStencil = 0;
	unsigned int MSAA = 1;
	bool ShaderResource = false;


	AssetOwner Owner;
};

struct FPDepthStencilArrayData
{
	void* Texture = nullptr;
	std::vector<void*> DepthStencilView;
	void* ShaderResourceView = nullptr;
	void* StencilShaderResourceView = nullptr;
};


//DepthStencilState
enum class FPDepthWriteMask
{
	ZERO,
	ALL,

	FPDepthWriteMask_MAX_
};

FPDepthWriteMask StringToFPDepthWriteMask(const std::string& Name);

enum class FPCompraisonFunc
{
	NERVER,
	LESS,
	EQUAL,
	LESS_EQUAL,
	GREATER,
	NOT_EQUAL,
	GREATER_EQUAL,
	ALWAYS,

	FPDepthFunc_MAX_
};

FPCompraisonFunc StringToFPCompraisonFunc(const std::string& Name);

enum  class FPStencilOP
{
	KEEP,
	ZERO,
	REPLACE,
	INCR_SAT,
	DECR_SAT,
	INVERT,
	INCR,
	DECR,

	FPStencilFailOp_MAX_
};

FPStencilOP StringToFPStencilOP(const std::string& Name);

struct FPDepthStencilStateInfo
{
	std::string Name;
	bool DepthEnable;
	FPDepthWriteMask DepthWriteMask;
	FPCompraisonFunc DepthFunc;

	bool StencilEnable;
	unsigned int StencilReadMask;
	unsigned int StencilWriteMask;

	FPStencilOP FrontFaceStencilFailOp;
	FPStencilOP FrontStencilDepthFailOp;
	FPStencilOP FrontStencilPassOp;
	FPCompraisonFunc FrontStencilFunc;

	FPStencilOP BackFaceStencilFailOp;
	FPStencilOP BackStencilDepthFailOp;
	FPStencilOP BackStencilPassOp;
	FPCompraisonFunc BackStencilFunc;

	AssetOwner Owner;
};

struct FPDepthStencilStateData
{
	void* DepthStencilData;
};


//RasterizerState
enum class FPFillMode
{
	SOLID,
	WIREFRAME,

	FPFillMode_MAX_
};

FPFillMode StringToFPFillMode(const std::string& Name);

enum class FPCullMode
{
	NONE,
	FRONT,
	BACK,

	FPCullMode_MAX_
};

FPCullMode StringToFPCullMode(const std::string& Name);

struct FPRasterizerStateInfo
{
	std::string Name;

	FPFillMode FillMode;
	FPCullMode CullMode;
	bool FrontCounterClockwise;

	int DepthBias;
	float DepthBiasClamp;
	float SlopeScaledDepthBias;

	bool DepthClipEnable;
	bool ScissorEnable;
	bool MultisampleEnable;
	bool AntialiasedLineEnable;

	AssetOwner Owner;
};

struct FPRasterizerStateData
{
	void* RasterizerStateData;
};

struct FPSamplerStateInfo
{
	std::string Name;

	FPSamplerFilterType Filter;
	FPTextureAddressMode AddressU;
	FPTextureAddressMode AddressV;
	FPTextureAddressMode AddressW;

	//ComparisonBiLinear(PCF) Filter에서만 사용
	FPCompraisonFunc ComparisonFunc;
	FPVector4 BorderColor;

	unsigned int MaxAnisotropy;
	float MinLOD;
	float MaxLOD;
	float MipLODBias;

	AssetOwner Owner;
};

struct FPSamplerStateData
{
	void* SamplerStateData = nullptr;
};


//BlendState
struct FPRenderTargetBlendDesc
{
	bool BlendEnable;
	FPBlend SrcBlend;
	FPBlend DestBlend;
	FPBlendOp BlendOp;

	FPBlend SrcBlendAlpha;
	FPBlend DestBlendAlpha;
	FPBlendOp BlendOpAlpha;

	std::vector<FPColorWriteMask> RenderTargetWriteMask;
};

struct FPBlendStateInfo
{
	std::string Name;

	bool AlphaToCoverageEnable;
	bool IndependentBlendEnable;

	//IndependentBlendEnable이 false면 RenderTargets[0]만 사용
	std::vector<FPRenderTargetBlendDesc> RenderTargets;

	AssetOwner Owner;
};

struct FPBlendStateData
{
	void* BlendStateData;
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
	Texture2DArray,

	RenderTargetTexture2D,
	DepthStencilTexture2D,
	StencilTexture2D,

	CubeMap,

	//MSAA Texture2DMS resource (reflection-only, never set directly via JSON)
	Texture2DMS,

	FPTextureType_MAX_
};

FPTextureType StringToTextureType(const std::string& Name);

enum class FPShaderType
{
	VS,
	PS,

	None
};

FPShaderType StringToFPShaderType(const std::string& ShaderType);

enum class FPMaterialDetail
{
	Opaque,
	Masked,
	Translucent,

	FPMaterialDetail_MAX_
};

FPMaterialDetail StringToFPMaterialDetail(const std::string& Name);

//////Material Data 구조체////////

struct FPReflectionParameterInfo
{
	FPShaderType Type = FPShaderType::PS;
	std::string ConstantBufferName;
	std::string ParameterName;
	size_t BufferOffset = 0;
	unsigned int Components = 0;
	unsigned int ArrayCount = 1; // 배열 요소 개수
	size_t ArrayStride = 0;      // 다음 요소까지의 바이트 간격
};

struct FPReflectionResourceInfo
{
	std::string Name;
	std::string AssetName;
	FPTextureType TextureType = Texture2D;
	unsigned int Slot = 0;
	unsigned int BindCount = 1;
};

struct FPReflectionSamplerInfo
{
	std::string Name;
	std::string SamplerType = "Filter_Tri_Linear_Sampler_CLAMP";
	unsigned int Slot = 0;
	unsigned int BindCount = 1;
};

struct FPShaderReflectionData
{
	std::vector<FPReflectionParameterInfo> Parameters;
	std::vector<FPReflectionResourceInfo> Resources;
	std::vector<FPReflectionSamplerInfo> Samplers;
};

//////////////////////////////////////////////////////////

//실수 파라이터 데이터 구조체
struct FPFloatParameterData
{
	FPShaderType Type = FPShaderType::PS;
	std::string ConstantBufferName; // 예: SurfaceCB
	std::string ParameterName;      // 예: TintColor 또는 Material.BaseColor
	std::vector<float> Values;
};

// 텍스처 SRV(t#)와 샘플러 상태(s#)는 서로 독립적으로 설정
struct FPTextureParameterData
{
	FPShaderType Type = FPShaderType::PS;
	std::string Parameter;							// HLSL 텍스처 변수 이름
	std::string Name;								// 로딩한 텍스처 에셋 이름
	FPTextureType TextureType = Texture2D;
	unsigned int ArrayIndex = 0;					// HLSL 리소스 배열 인덱스. Texture2DArray의 슬라이스가 아님
};

struct FPSamplerParameterData
{
	FPShaderType Type = FPShaderType::PS;
	std::string Parameter;							// HLSL 샘플러 변수 이름
	std::string SamplerType = "Filter_Tri_Linear_Sampler_Wrap";
	unsigned int ArrayIndex = 0;					// HLSL SamplerState 배열 원소 번호
};

//Material 데이터 구조체
struct FPMaterialData
{
	std::string Name;
	std::string VertexShader;
	std::string PixelShader;
	AssetOwner Owner;

	bool TwoSided;

	std::string BlendState;
	FPMaterialDetail Detail = FPMaterialDetail::Opaque;
	bool bIsSky = false;
	bool bUsePlanarForwardReflections = false;

	std::vector<FPFloatParameterData> Parameters;
	std::vector<FPTextureParameterData> NamedTextures;
	std::vector<FPSamplerParameterData> NamedSamplers;
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

struct RenderItem
{
	int* Priority = nullptr;
	bool* Active = nullptr;

	//FPActor*
	void* OwnActor = nullptr;

	std::string* MeshPath = nullptr;
	
	//핵심 데이터
	bool* TwoSided = nullptr;

	std::string* BlendState = nullptr;
	FPMaterialDetail* Detail = nullptr;
	bool* bIsSky = nullptr;
	bool* bUsePlanarForwardReflections = nullptr;
	int* StencilRefValue = nullptr;
	bool* bCustomDepthStencil = nullptr;
	bool* bCastShadow = nullptr;

	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	std::string* VertexShaderPath = nullptr;
	std::string* PixelShaderPath = nullptr;

	std::vector<FPReflectionResourceInfo>* Resources = nullptr;
	std::vector<FPReflectionSamplerInfo>* Samplers = nullptr;

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

class FPMaterialInterface;

struct CameraItem
{
	//카메라의 위치
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	FPMatrix* View = nullptr;
	FPMatrix* Projection = nullptr;

	bool* Active = nullptr;			//카메라 사용 여부

	//PostProcess Material (nullptr = 없음). PostProcess Material을 가진 Camera는 PostProcessList에 등록됨
	FPMaterialInterface** PostProcessMaterial = nullptr;

	//Projection 설정
	float* Fov = nullptr;
	float* Aspect = nullptr;
	float* Zn = nullptr;
	float* Zf = nullptr;
	FPProjectionMode* ProjectionMode = nullptr;
	float* OrthoWidth = nullptr;
};

//SceneCapture Component
struct SceneCaptureItem
{
	//카메라의 위치
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	FPMatrix* View = nullptr;
	FPMatrix* Projection = nullptr;

	bool* Active = nullptr;			//카메라 사용 여부

	std::string* RenderTarget = nullptr;								//사용하는 RenderTarget 이름
	std::string* DepthStencil = nullptr;

	bool* CaptureEveryFrame = nullptr;
	bool* CaptureThisFrame = nullptr;

	std::vector<std::string>* HiddenActors = nullptr;

	//Projection 설정
	float* Fov = nullptr;
	float* Aspect = nullptr;
	float* Zn = nullptr;
	float* Zf = nullptr;
	FPProjectionMode* ProjectionMode = nullptr;
	float* OrthoWidth = nullptr;
};

//Planar Reflection Component
struct PlanarReflectionItem
{
	//Capture Camera
	FPMatrix* Location = nullptr;
	FPMatrix* Rotation = nullptr;
	FPMatrix* Scale = nullptr;

	bool* Active = nullptr;

	std::string* RenderTarget = nullptr;
	std::string* DepthStencil = nullptr;

	bool* CaptureEveryFrame = nullptr;
	bool* CaptureThisFrame = nullptr;

	std::vector<std::string>* HiddenActors = nullptr;

	//Reflection Plane / Matrix
	FPVector4* Plane = nullptr;
	FPMatrix* ReflectionMatrix = nullptr;
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
};

struct FPSkyLightRenderItem
{
	bool* Active = nullptr;

	float* LightIntensity = nullptr;
	FPVector4* LightColor = nullptr;

	std::string* CubeMap = nullptr;
};

struct FPCascadeShadowRenderItem
{
	bool* CastShadow = nullptr;

	FPVector3* Direction = nullptr;

	unsigned int* CascadeCount = nullptr;
	unsigned int* ShadowResolution = nullptr;

	float* DynamicShadowDistance = nullptr;
	float* CascadeSplitLambda = nullptr;
	float* CascadeTransitionFraction = nullptr;
	float* ShadowDistanceFadeoutFraction = nullptr;
	float* ShadowBias = nullptr;
	float* ShadowSlopeBias = nullptr;
	float* DistanceMargin = nullptr;

	std::string* CascadeShadowMapName = nullptr;
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

	struct CameraItem
	{
		//카메라의 위치
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		FPMatrix View;
		FPMatrix Projection;

		//Projection 설정
		float Fov;
		float Aspect;
		float Zn;
		float Zf;
		FPProjectionMode ProjectionMode;
		float OrthoWidth;
	};

	//PostProcess 목록 항목 (PostProcess Material을 가진 Camera)
	struct PostProcessItem
	{
		FPMaterialInterface* Material = nullptr;		//Camera가 가진 PostProcess Material
	};

	struct UIContextItem
	{
		int Priority;
		bool active;
		int x;
		int y;
		FPVector4 color;
		std::basic_string<TCHAR> msg;
	};

	struct alignas(16) FPSkyLightDataItem
	{
		FPVector4 LightColor;
		float LightIntensity;
	};

	struct FPSkyLightRenderItem
	{
		FPSkyLightDataItem SkyLightDataItem;

		std::string CubeMap;
	};

	struct FPCascadeShadowRenderItem
	{
		bool CastShadow;

		FPVector3 Direction;

		unsigned int CascadeCount;
		unsigned int ShadowResolution;

		float DynamicShadowDistance;
		float CascadeSplitLambda;
		float CascadeTransitionFraction;
		float ShadowDistanceFadeoutFraction;
		float ShadowBias;
		float ShadowSlopeBias;
		float DistanceMargin;

		std::string CascadeShadowMapName;
	};

	struct FPCascadeNearFar
	{
		float Near;
		float Far;
	};

	struct FPCascadeFrustum
	{
		std::vector<FPVector3> NDCVertex;
		FPVector3 Center;
		FPMatrix InvCascadeVP;
	};

	//한 Light가 가질 수 있는 최대 Cascade 개수 (CascadeShadow.hlsli의 MAX_CASCADE와 일치해야 함)
	constexpr unsigned int Cascade_MAX = 8;

	//CascadeShadow.hlsli의 CascadeShadowConstBuffer(register b13)와 1:1 대응되는 구조체
	struct alignas(16) CascadeShadowConstBuffer
	{
		FPMatrix mLightViewProj[Cascade_MAX];           //Shadow Map 투영 좌표 계산

		//HLSL에서 float 배열의 각 원소는 16B 단위로 패킹되므로 x만 사용
		FPVector4 CascadeNear[Cascade_MAX];             //Cascade 시작 깊이
		FPVector4 CascadeFar[Cascade_MAX];              //Cascade 종료 깊이 및 선택
		FPVector4 CascadeDepthRange[Cascade_MAX];       //Light Projection의 Far-Near, Bias 정규화

		unsigned int CascadeCount;                      //Cascade 개수
		float DynamicShadowDistance;                    //최대 그림자 적용 거리
		float CascadeTransitionFraction;                //Cascade Blending

		float ShadowDistanceFadeoutFraction;            //거리 기반 Shadow Fade Out
		float ShadowBias;                               //깊이 비교 보정
		float ShadowSlopeBias;                          //Slope-Scaled Bias 계수
		unsigned int CastShadow;                        //그림자 적용 여부
		float Padding;                                  //16B 배수 유지용 Padding

		FPVector4 LightDirectionView;                   //Slope-Scaled Bias 계산용 View Space 빛 방향 (xyz만 사용)
	};

	constexpr unsigned int Light_MAX = 64;

	struct alignas(16) LightDataItem
	{
		unsigned long LightType;
		float padding[3];
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
		unsigned long LightCnt = 0;
		float Padding[3];
		LightDataItem Light[Light_MAX];
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

	//Planar Reflection Clip Plane 상수 버퍼 (고정 슬롯 b13)
	struct alignas(16) ClipPlanarReflectionConstBuffer
	{
		FPVector4 GlobalClipPlane = FPVector4(0.0f, 0.0f, 0.0f, 0.0f);
		unsigned int ClipPlaneEnabled = 0;
		float Padding[3] = {};
		FPMatrix ReflectionViewProj;
	};

	//RenderTarget 이름별로 캐싱된 ReflectionMatrix/Plane (Renderer::PlanarReflectionCache 참고)
	struct PlanarReflectionCacheData
	{
		FPMatrix ReflectionMatrix;
		FPVector4 Plane;
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
		std::string SamplerType;
		FPTextureType TextureType;
	};

	struct FPSamplerItem
	{
		int Slot;
		std::string SamplerType;
	};

	struct MeshRenderItem
	{
		int Priority;

		std::string ActorName;
		std::string MeshPath;

		//주요 정보
		bool TwoSided;

		std::string BlendState;
		FPMaterialDetail Detail;
		bool bIsSky;
		bool bUsePlanarForwardReflections;
		int StencilRefValue;
		bool bCustomDepthStencil;
		bool bCastShadow;

		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string VertexShaderPath;
		std::string PixelShaderPath;

		std::vector<FPReflectionResourceInfo> Resources;
		std::vector<FPReflectionSamplerInfo> Samplers;

		FPConstantBufferRenderData VertexConstBuffer;
		FPConstantBufferRenderData PixelConstBuffer;
	};

	//SceneCapture Component
	struct SceneCaptureItem
	{
		//카메라의 위치
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		FPMatrix View;
		FPMatrix Projection;

		std::string RenderTarget;								//사용하는 RenderTarget 이름
		std::string DepthStencil;

		std::vector<std::string> HiddenActors;

		//Projection 설정
		float Fov;
		float Aspect;
		float Zn;
		float Zf;
		FPProjectionMode ProjectionMode;
		float OrthoWidth;
	};

	struct PlanarReflectionItem
	{
		FPMatrix Location;
		FPMatrix Rotation;
		FPMatrix Scale;

		std::string RenderTarget;
		std::string DepthStencil;

		std::vector<std::string> HiddenActors;

		FPVector4 Plane;
		FPMatrix ReflectionMatrix;
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
