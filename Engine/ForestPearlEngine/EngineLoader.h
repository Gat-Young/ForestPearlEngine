#pragma once
#include "FPGameInstance.h"
#include "FPAssetLoader.h"

void LoadEngineAssets()
{
	FPAssetLoader* AssetLoader = static_cast<FPAssetLoader*>(FPGameInstance::Get().GetAssetLoader());

	//BuiltIn Mesh Load
	AssetLoader->LoadBuiltInStaticMesh("Quad2D_Mesh.json");

	//BuiltIn Vertex/Pixel Shader Load
	AssetLoader->LoadBuiltInVertexShader("DefaultQuad2DVertexShader.json");
	AssetLoader->LoadBuiltInPixelShader("DefaultQuad2DPixelShader.json");

	//BackBuffer DepthStencil BuiltIn Load
	AssetLoader->LoadBuiltInDepthStencil("BackBufferDepthStencil.json");

	//Custom DepthStencil BuiltIn Load
	AssetLoader->LoadBuiltInDepthStencil("CustomDepthStencil.json");

	//PlanarReflection DepthStencil BuiltIn Load
	AssetLoader->LoadBuiltInDepthStencil("DefaultPlanarReflectionDepthStencil.json");

	//BuiltIn RenderTarget Load
	AssetLoader->LoadBuiltInRenderTargetTexture2D("MeshRenderPass_RenderTargetTexture2D-BuiltIn.json");
	AssetLoader->LoadBuiltInRenderTargetTexture2D("GizmoRenderPass_RenderTargetTexture2D-BuiltIn.json");
	AssetLoader->LoadBuiltInRenderTargetTexture2D("PostProcessRenderPass_RenderTargetTexture2D-BuiltIn.json");
	AssetLoader->LoadBuiltInRenderTargetTexture2D("DefaultPlanarReflectionRenderTarget.json");

	//Default Vertex/Pixel Shader
	AssetLoader->LoadVertexShader("DefaultVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultPixelShader.json", AssetOwner::Engine);
	AssetLoader->LoadVertexShader("DefaultGizmoVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultGizmoPixelShader.json", AssetOwner::Engine);
	AssetLoader->LoadVertexShader("NormalDebugVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("NormalDebugPixelShader.json", AssetOwner::Engine);
	AssetLoader->LoadVertexShader("DefaultSkyBoxQuadVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultSkyBoxQuadPixelShader.json", AssetOwner::Engine);
	AssetLoader->LoadVertexShader("DefaultPlanarReflectionVertexShader.json", AssetOwner::Engine);
	AssetLoader->LoadPixelShader("DefaultPlanarReflectionPixelShader.json", AssetOwner::Engine);

	//StaticMesh Load
	AssetLoader->LoadStaticMesh("Cube_StaticMesh.json", AssetOwner::Engine);
	AssetLoader->LoadStaticMesh("Sphere_StaticMesh.json", AssetOwner::Engine);
	AssetLoader->LoadStaticMesh("Plane_StaticMesh.json", AssetOwner::Engine);
	AssetLoader->LoadStaticMesh("SkyBoxQuad_StaticMesh.json", AssetOwner::Engine);

	//Texture Load
	AssetLoader->LoadTexture("Default_Texture512.json", AssetOwner::Engine);
	AssetLoader->LoadTexture("Checker_bmp.json", AssetOwner::Engine);
	AssetLoader->LoadTexture("Checker_dds.json", AssetOwner::Engine);
	AssetLoader->LoadTexture("rustig_koppie_puresky_2k.json", AssetOwner::Engine);

	//Material Load
	AssetLoader->LoadMaterial("DefaultMaterial.json", AssetOwner::Engine);
	AssetLoader->LoadMaterial("DefaultSkyBoxMaterial.json", AssetOwner::Engine);

	//DepthStencilState Load
	AssetLoader->LoadDepthStencilState("DS_DEPTH_ON.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_OFF.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_WRITE_OFF.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_ON_STENCIL_ON.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_OFF_STENCIL_ON.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_WRITE_OFF_STENCIL_ON.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_ON_STENCIL_EQUAL_KEEP.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_ON_STENCIL_EQUAL_INCR.json", AssetOwner::Engine);
	AssetLoader->LoadDepthStencilState("DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP.json", AssetOwner::Engine);

	//BlendState Load
	AssetLoader->LoadBlendState("BS_DEFAULT.json", AssetOwner::Engine);
	AssetLoader->LoadBlendState("BS_ADD.json", AssetOwner::Engine);

	//RasterizerState Load
	AssetLoader->LoadRasterizerState("RS_SOLID.json", AssetOwner::Engine);
	AssetLoader->LoadRasterizerState("RS_WIREFRM.json", AssetOwner::Engine);
	AssetLoader->LoadRasterizerState("RS_CULLBACK.json", AssetOwner::Engine);
	AssetLoader->LoadRasterizerState("RS_CULLFRONT.json", AssetOwner::Engine);
	AssetLoader->LoadRasterizerState("RS_WIRECULLBACK.json", AssetOwner::Engine);
	AssetLoader->LoadRasterizerState("RS_WIRECULLFRONT.json", AssetOwner::Engine);
};