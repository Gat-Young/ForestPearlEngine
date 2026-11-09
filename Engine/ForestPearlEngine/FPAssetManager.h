#pragma once
#include <string>
#include <unordered_map>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "Renderers/FPRenderingCommon.h"
#include "Libraries/Ufbx/ufbx.h"

class FPStaticMesh;
class FPMaterialInterface;

class FPAssetManager : public FPGameInstanceSubSystem
{
private:
	//GameSetting Data
	//무조건 1개만 보유
	FPGameSettingData* GameSettingData = nullptr;

	//Data List
	std::unordered_map<std::string, FPTextureInfo> LoadedTextureData;
	std::unordered_map<std::string, FPTextureArrayInfo> LoadedTextureArrayData;
	std::unordered_map<std::string, FPMaterialData> LoadedMaterialData;
	std::unordered_map<std::string, FPStaticMeshData> LoadedStaticMeshData;
	std::unordered_map<std::string, FPVertexShaderData> LoadedVertexShaderData;
	std::unordered_map<std::string, FPPixelShaderData> LoadedPixelShaderData;
	std::unordered_map<std::string, FPMeshData> LoadedMeshData;
	std::unordered_map<std::string, std::vector<FPActorData> > LevelData;
	std::unordered_map<std::string, std::string> GameModeData;

	//Resource List (실제 GPU 객체)
	std::unordered_map<std::string, FPVertexBufferData> MeshPositionVertexBuffer;
	std::unordered_map<std::string, FPVertexBufferData> MeshNormalVertexBuffer;
	std::unordered_map<std::string, FPVertexBufferData> MeshUVVertexBuffer;
	std::unordered_map<std::string, FPVertexBufferData> MeshColorVertexBuffer;
	std::unordered_map<std::string, FPIndexBufferData> MeshIndexBuffer;
	std::unordered_map<std::string, FPVertexBufferData> MeshTangentVertexBuffer;

	//Shader
	std::unordered_map<std::string, std::tuple<void*, void*, void*> > VertexShaderData;
	std::unordered_map<std::string, std::pair<void*, void*> > PixelShaderData;

	//Texture
	std::unordered_map<std::string, FPTextureData> TextureData;
	std::unordered_map<std::string, FPTextureArrayData> TextureArrayData;

	//RenderTarget
	std::unordered_map<std::string, FPRenderTargetTexture2DInfo> LoadedRenderTargetTexture2DData;
	std::unordered_map<std::string, FPRenderTargetData> RenderTargetData;

	//DepthStencil
	std::unordered_map<std::string, FPDepthStencilInfo> LoadedDepthStencilData;
	std::unordered_map<std::string, FPDepthStencilData> DepthStencilData;

	//DepthStencill State
	std::unordered_map<std::string, FPDepthStencilStateInfo> LoadedDepthStencilStateData;
	std::unordered_map<std::string, FPDepthStencilStateData> DepthStencilStateData;

	bool CalculateTriangleTangent(const POSITION_VERTEX& P0, const POSITION_VERTEX& P1, const POSITION_VERTEX& P2,
									const UV_VERTEX& UV0, const UV_VERTEX& UV1, const UV_VERTEX& UV2,
										FPVector3& OutTangent, FPVector3& OutBinormal);

	bool SameTangentAttributes(const FPMeshData& Mesh, uint32_t A, uint32_t B);
	
	float CalculateCornerAngle(const FPMeshData& Mesh, size_t Base, size_t LocalCorner);

	bool GenerateTangents(FPMeshData& MeshData);

	std::string ConvertUfbxString(ufbx_string String);
	struct FPMeshData ConvertUfbxMesh(const ufbx_mesh* Mesh, const ufbx_node* Node);

public:
	FPAssetManager() = default;
	~FPAssetManager() = default;

	//모든 Asset Data를 객체 생성
	void AllAssetDataCreate();

	//Gizmo Vertex Buffer 생성
	void MakeGizmoVertexBuffer(std::vector<struct VERTEX>& Mesh, const std::string& GizemoMeshPath);

	//Buffer 생성
	void MakeMeshBuffer(FPMeshData& MeshData);
	void* MakePositionVertexBuffer(std::vector<struct POSITION_VERTEX>& Mesh);
	void* MakeColorVertexBuffer(std::vector<struct COLOR_VERTEX>& Mesh);
	void* MakeNormalVertexBuffer(std::vector<struct NORMAL_VERTEX>& Mesh);
	void* MakeUVVertexBuffer(std::vector<struct UV_VERTEX>& Mesh);
	void* MakeIndex16BitBuffer(std::vector<struct INDEX_16BIT>& Mesh);
	void* MakeIndex32BitBuffer(std::vector<struct INDEX_32BIT>& Mesh);
	void* MakeTangentVertexBuffer(std::vector<struct TANGENT_VERTEX>& Mesh);

	//Shader 생성
	void MakeVertexShader(FPVertexShaderData& VertexShaderData);
	void MakePixelShader(FPPixelShaderData& PixelShaderData);

	//FBX Load
	void LoadFbxData(FPStaticMeshData& StaticMeshData);

	//Mesh
	void AddMeshData(std::string ModelNodeName, FPMeshData& MeshData);
	FPMeshData& GetMeshData(std::string FbxPath);
	bool HasMeshData(std::string ModelNodeName);

	//각 필요한 정점 버퍼에 값 추가

	//Position
	void AddPositionVertexBuffer(std::string FbxPath, FPVertexBufferData& PositionVertexBuffer);
	void AddPositionVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasPositionBuffer(std::string MeshPath);
	FPVertexBufferData GetPositionVertexBuffer(std::string MeshPath);

	//Color
	void AddColorVertexBuffer(std::string FbxPath, FPVertexBufferData& ColorVertexBuffer);
	void AddColorVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasColorBuffer(std::string MeshPath);
	FPVertexBufferData GetColorVertexBuffer(std::string MeshPath);

	//Normal
	void AddNormalVertexBuffer(std::string FbxPath, FPVertexBufferData& NormalVertexBuffer);
	void AddNormalVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasNormalBuffer(std::string MeshPath);
	FPVertexBufferData GetNormalVertexBuffer(std::string MeshPath);

	//UV
	void AddUVVertexBuffer(std::string FbxPath, FPVertexBufferData& UVVertexBuffer);
	void AddUVVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasUVBuffer(std::string MeshPath);
	FPVertexBufferData GetUVVertexBuffer(std::string MeshPath);

	//Index
	void AddIndexBuffer(std::string FbxPath, FPIndexBufferData& PositionVertexBuffer);
	void AddIndexBuffer(std::string FbxPath, void* IndexBuffer, unsigned long IBSize, unsigned long IBCnt, unsigned long FaceCnt, unsigned long Stride, unsigned long Offset);
	bool HasIndexBuffer(std::string MeshPath);
	FPIndexBufferData GetIndexBuffer(std::string MeshPath);

	//Tangent
	void AddTangentVertexBuffer(std::string FbxPath, FPVertexBufferData& TangentVertexBuffer);
	void AddTangentVertexBuffer(std::string FbxPath, void* VertexBuffer, unsigned long VBSize, unsigned long VBCnt, unsigned long Stride, unsigned long Offset);
	bool HasTangentBuffer(std::string MeshPath);
	FPVertexBufferData GetTangentVertexBuffer(std::string MeshPath);

	//StaticMesh
	FPStaticMesh* GetStaticMeshData(std::string StaticMeshName);
	FPStaticMeshData& GetStaticMeshData_(std::string StaticMeshName);
	void AddStaticMeshData(std::string StaticMeshName, FPStaticMeshData* StaticMeshData);
	bool HasStaticMeshData(std::string StaticMeshName);

	//Texture
	FPTextureData& GetTextureData(std::string TextureName);
	void AddTextureData(std::string TextureName, FPTextureData& TextureData);
	bool HasTextureData(std::string TextureName);
	void MakeTexture(FPTextureInfo& TextureInfo);

	//Texture Array
	FPTextureArrayData& GetTextureArrayData(std::string TextureArrayName);
	void AddTextureArrayData(std::string TextureArrayName, FPTextureArrayData& TextureArrayData);
	bool HasTextureArrayData(std::string TextureArrayName);
	void MakeTextureArray(FPTextureArrayInfo& TextureInfo);

	//Texture Info
	FPTextureInfo& GetTextureInfoData(std::string TextureName);
	void AddTextureInfoData(std::string TextureName, FPTextureInfo& TextureData);
	bool HasTextureInfoData(std::string TextureName);

	//Texture Array Info
	FPTextureArrayInfo& GetTextureArrayInfoData(std::string TextureArrayName);
	void AddTextureArrayInfoData(std::string TextureArrayName, FPTextureArrayInfo& TextureArrayData);
	bool HasTextureArrayInfoData(std::string TextureArrayName);
	
	//Render Target Texture 2D Info
	FPRenderTargetTexture2DInfo& GetRenderTargetTexture2DInfoData(std::string RenderTargetTexture2DName);
	void AddRenderTargetTexture2DData(std::string RenderTargetTexture2DName, FPRenderTargetTexture2DInfo& RenderTargetTexture2DData);
	bool HasRenderTargetTexture2DData(std::string RenderTargetTexture2DName);

	//Render Target Data
	FPRenderTargetData& GetRenderTargetData(std::string RenderTargetName);
	void AddRenderTargetData(std::string RenderTargetName, FPRenderTargetData& RenderTargetData);
	bool HasRenderTargetData(std::string RenderTargetName);
	void MakeRenderTarget(FPRenderTargetTexture2DInfo& RenderTargetInfo);

	//DepthStencil Info
	FPDepthStencilInfo& GetDepthStencilInfoData(std::string DepthStencilName);
	void AddDepthStencilInfoData(std::string DepthStencilName, FPDepthStencilInfo& DepthStencilyData);
	bool HasDepthStencilInfoData(std::string DepthStencilName);

	//DepthStencil Data
	FPDepthStencilData& GetDepthStencilData(std::string DepthStencilName);
	void AddDepthStencilData(std::string DepthStencilName, FPDepthStencilData& TextureData);
	bool HastDepthStencilData(std::string DepthStencilName);
	void MaketDepthStencil(FPDepthStencilInfo& DepthStecilInfo);


	//DepthStencilState Info
	FPDepthStencilStateInfo& GetDepthStencilStateInfoData(std::string DepthStencilStateName);
	void AddDepthStencilStateInfoData(std::string DepthStencilStateName, FPDepthStencilStateInfo& DepthStencilStateData);
	bool HasDepthStencilStateInfoData(std::string DepthStencilStateName);

	//DepthStencilState Data
	FPDepthStencilStateData& GetDepthStencilStateData(std::string DepthStencilStateName);
	void AddDepthStencilStateData(std::string DepthStencilStateName, FPDepthStencilStateData& DepthStencilStateData);
	bool HasDepthStencilStateData(std::string DepthStencilStateName);
	void MakeDepthStencilState(FPDepthStencilStateInfo& DepthStencilStateInfo);

	//Material Info
	FPMaterialData& GetMaterialData(std::string MaterialName);
	void AddMaterialData(std::string MaterialName, FPMaterialData& MaterialData);
	bool HasMaterialData(std::string MaterialName);

	//Material
	FPMaterialInterface* MakeMaterial(std::string MaterialName);

	//GameSetting
	bool HasGameSettingData();
	void UpdateGameSettingData(FPGameSettingData& GameSettingData);
	FPGameSettingData* GetGameSettingData();

	//Level
	bool HasLevelData(std::string LevelName);
	void AddLevelData(std::string LevelName, FPActorData* ActorData);
	std::vector<FPActorData>& GetLevelData(std::string LevelName);

	//GameMode
	void AddGameModeData(std::string LevelName, std::string GameModeName);
	std::string GetGameModeData(std::string LevelName);


	//Shader
	//Vertext
	FPVertexShaderData& GetVertexShaderData(std::string VertexShaderName);
	void AddVertexShaderData(std::string VertexShaderName, FPVertexShaderData& VertexShaderData);
	bool HasVertexShaderData(std::string VertexShaderName);

	//Shader객체 생성
	bool HasVertexShader(std::string ShaderName);
	void AddVertexShader(std::string ShaderName, void* VertexShader, void* VSCode, void* VBLayout);
	//VertexShader , VSCode, VBLayout
	std::tuple<void*, void*, void*> GetVertexShader(std::string ShaderName);

	//pixel
	FPPixelShaderData& GetPixelShaderData(std::string PixelShaderName);
	void AddPixelShaderData(std::string PixelShaderName, FPPixelShaderData& PixelShaderData);
	bool HasPixelShaderData(std::string PixelShaderName);

	//Shader 객체 생성
	bool HasPixelShader(std::string ShaderName);
	void AddPixelShader(std::string ShaderName, void* PixelShader, void* PSCode);
	//PixelShader, PSCode
	std::pair<void*, void*> GetPixelShader(std::string ShaderName);
};