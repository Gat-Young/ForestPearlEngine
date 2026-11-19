#include "WaterAssets.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/Utility/FPPathManager.h"
#include "ForestPearlEngine/Renderers/RenderingDevice.h"
#include "ForestPearlEngine/Shader/ShaderFactory.h"
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

namespace
{
    FPAssetManager& Assets()
    {
        return *static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
    }

    void Check(HRESULT result, const char* action)
    {
        if (FAILED(result)) throw std::runtime_error(std::string("Water: ") + action);
    }

    ComPtr<ID3DBlob> Compile(const char* name, bool vertex)
    {
        // Runtime source loading avoids changes to any existing .vcxproj shader build rules.
        const std::string filename = std::string(name) + (vertex ? ".vsh" : ".psh");
        const auto path = std::filesystem::path(FPPathManager::Get().GetAssetPath("ShaderResource/" + filename));
        ComPtr<ID3DBlob> code, errors;
        const HRESULT result = D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
            vertex ? "VS_Main" : "PS_Main", vertex ? "vs_5_0" : "ps_5_0",
            D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS, 0,
            code.GetAddressOf(), errors.GetAddressOf());
        if (errors) std::cerr << static_cast<const char*>(errors->GetBufferPointer());
        Check(result, filename.c_str());
        return code;
    }

    void LoadVertexShader(const char* name)
    {
        if (Assets().HasVertexShader(name)) return;
        auto code = Compile(name, true);
        ComPtr<ID3D11VertexShader> shader;
        ComPtr<ID3D11InputLayout> layout;
        auto* device = RenderingDevice::GetRenderingDevice().GetDXDevice();
        Check(device->CreateVertexShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, shader.GetAddressOf()), "create vertex shader");
        void* rawLayout = nullptr;
        Check(ShaderFactory::GetShaderFactory().CreateInputLayout(code.Get(), &rawLayout), "create input layout");
        layout.Attach(static_cast<ID3D11InputLayout*>(rawLayout));
        FPVertexShaderData info{};
        info.Name = name;
        info.Path = std::string(name) + ".vsh";
        info.InputLayout = "Default";
        info.Owner = AssetOwner::User;
        Assets().AddVertexShaderData(name, info);
        Assets().AddVertexShader(name, shader.Detach(), code.Detach(), layout.Detach());
    }

    void LoadPixelShader(const char* name)
    {
        if (Assets().HasPixelShader(name)) return;
        auto code = Compile(name, false);
        ComPtr<ID3D11PixelShader> shader;
        auto* device = RenderingDevice::GetRenderingDevice().GetDXDevice();
        Check(device->CreatePixelShader(code->GetBufferPointer(), code->GetBufferSize(), nullptr, shader.GetAddressOf()), "create pixel shader");
        FPPixelShaderData info{};
        info.Name = name;
        info.Path = std::string(name) + ".psh";
        info.Owner = AssetOwner::User;
        Assets().AddPixelShaderData(name, info);
        Assets().AddPixelShader(name, shader.Detach(), code.Detach());
    }

    void RegisterMesh(const char* name, FPMeshData& mesh, const char* material)
    {
        mesh.Name = mesh.NodeName = name;
        Assets().AddMeshData(name, mesh);
        Assets().MakeMeshBuffer(mesh);
        FPStaticMeshData info{};
        info.Name = info.NodeName = info.MeshPath = name;
        info.MeshTopology = Topology::TRIANGLELIST;
        info.MaterialName = material;
        info.Owner = AssetOwner::User;
        Assets().AddStaticMeshData(name, &info);
    }

    void RegisterMeshes()
    {
        FPMeshData plane{};
        plane.Position_Vertices = {{-12, 0, -10}, {-12, 0, 14}, {12, 0, 14}, {12, 0, -10}};
        plane.Normal_Vertices.assign(4, {0, 1, 0});
        plane.Color_Vertices.assign(4, {1, 1, 1, 1});
        plane.UV_Vertices = {{0, 1}, {0, 0}, {1, 0}, {1, 1}};
        plane.Tangent_Vertices.assign(4, {1, 0, 0, 1});
        plane.Index = {{0, 1, 2}, {0, 2, 3}};
        RegisterMesh("WaterPlane", plane, "WaterMaterial");

        FPMeshData cube{};
        const auto face = [&cube](FPVector3 a, FPVector3 b, FPVector3 c, FPVector3 d, NORMAL_VERTEX normal)
        {
            const auto start = static_cast<unsigned long>(cube.Position_Vertices.size());
            for (auto p : {a, b, c, d})
            {
                cube.Position_Vertices.push_back({p.x, p.y, p.z});
                cube.Normal_Vertices.push_back(normal);
                cube.Color_Vertices.push_back({1, 1, 1, 1});
                cube.Tangent_Vertices.push_back({1, 0, 0, 1});
            }
            cube.UV_Vertices.insert(cube.UV_Vertices.end(), {{0, 1}, {0, 0}, {1, 0}, {1, 1}});
            cube.Index.push_back({start, start + 1, start + 2});
            cube.Index.push_back({start, start + 2, start + 3});
        };
        face({-.5f,-.5f,-.5f}, {-.5f,.5f,-.5f}, {.5f,.5f,-.5f}, {.5f,-.5f,-.5f}, {0,0,-1});
        face({.5f,-.5f,.5f}, {.5f,.5f,.5f}, {-.5f,.5f,.5f}, {-.5f,-.5f,.5f}, {0,0,1});
        face({-.5f,-.5f,.5f}, {-.5f,.5f,.5f}, {-.5f,.5f,-.5f}, {-.5f,-.5f,-.5f}, {-1,0,0});
        face({.5f,-.5f,-.5f}, {.5f,.5f,-.5f}, {.5f,.5f,.5f}, {.5f,-.5f,.5f}, {1,0,0});
        face({-.5f,.5f,-.5f}, {-.5f,.5f,.5f}, {.5f,.5f,.5f}, {.5f,.5f,-.5f}, {0,1,0});
        face({-.5f,-.5f,.5f}, {-.5f,-.5f,-.5f}, {.5f,-.5f,-.5f}, {.5f,-.5f,.5f}, {0,-1,0});
        RegisterMesh("WaterCube", cube, "WaterModelMaterial");
    }

    void AddParameter(FPMaterialData& material, const char* buffer, const char* name, std::vector<float> values)
    {
        FPFloatParameterData parameter{};
        parameter.Type = FPShaderType::PS;
        parameter.ConstantBufferName = buffer;
        parameter.ParameterName = name;
        parameter.Values = std::move(values);
        material.Parameters.push_back(std::move(parameter));
    }

    void RegisterMaterials()
    {
        FPMaterialData water{};
        water.Name = "WaterMaterial";
        water.VertexShader = "WaterVertexShader";
        water.PixelShader = "WaterPixelShader";
        water.Owner = AssetOwner::User;
        water.TwoSided = true;
        water.BlendState = "BS_DEFAULT";
        AddParameter(water, "WaterSettings", "FlowOffset", {0});
        AddParameter(water, "WaterSettings", "DistortionStrength", {.8f});
        AddParameter(water, "WaterSettings", "Tiling", {2});
        AddParameter(water, "WaterSettings", "ReflectionAmount", {.8f});
        AddParameter(water, "WaterSettings", "WaterColor", {.035f, .18f, .24f, 1});
        AddParameter(water, "WaterSettings", "EnvironmentAmount", {.65f});
        AddParameter(water, "WaterSettings", "EnvironmentDistortionScale", {6.0f});
        water.NamedTextures = {
            {FPShaderType::PS, "BumpMap", "WaterBump", FPTextureType::Texture2D, 0},
            {FPShaderType::PS, "ReflectionMap", "WaterReflection", FPTextureType::RenderTargetTexture2D, 0}};
        water.NamedSamplers = {
            {FPShaderType::PS, "BumpWrap", FPSamplerType::Filter_Bi_Linear_Sampler_Wrap, 0},
            {FPShaderType::PS, "ReflectionClamp", FPSamplerType::Filter_Bi_Linear_Sampler_CLAMP, 0}};
        Assets().AddMaterialData(water.Name, water);

        FPMaterialData model{};
        model.Name = "WaterModelMaterial";
        model.VertexShader = "WaterModelVertexShader";
        model.PixelShader = "WaterModelPixelShader";
        model.Owner = AssetOwner::User;
        // A reflected view reverses winding; handle it in this demo's material.
        model.TwoSided = true;
        model.BlendState = "BS_DEFAULT";
        AddParameter(model, "ModelSettings", "Tint", {1, 1, 1, 1});
        Assets().AddMaterialData(model.Name, model);
    }
}

void WaterAssets::RegisterAssets()
{
    RegisterMaterials();
    FPRenderTargetTexture2DInfo reflection{};
    reflection.Name = "WaterReflection";
    reflection.Type = "Texture2D";
    reflection.SizeX = 1280;
    reflection.SizeY = 800;
    reflection.ClearColor = {0, 0, 0, 0};
    reflection.Owner = AssetOwner::User;
    Assets().AddRenderTargetTexture2DData(reflection.Name, reflection);
    FPDepthStencilInfo depth{};
    depth.Name = "WaterReflectionDepth";
    depth.SizeX = reflection.SizeX;
    depth.SizeY = reflection.SizeY;
    depth.Owner = AssetOwner::User;
    Assets().AddDepthStencilInfoData(depth.Name, depth);
}

void WaterAssets::LoadShadersAndBump()
{
    // The standard JSON loader and AllAssetDataCreate load this PNG before actors initialize.
    if (!Assets().HasTextureData("WaterBump") || !Assets().GetTextureData("WaterBump").ShaderResourceView)
        throw std::runtime_error("Water: Water/WaterBump.json or Water/WaterBumps1.png could not be loaded.");
    if (!Assets().HasTextureData("rustig_koppie_puresky_2k") || !Assets().GetTextureData("rustig_koppie_puresky_2k").ShaderResourceView)
        throw std::runtime_error("Water: the built-in rustig_koppie_puresky_2k environment cubemap is not loaded.");

    LoadVertexShader("WaterVertexShader");
    LoadPixelShader("WaterPixelShader");
    LoadVertexShader("WaterModelVertexShader");
    LoadPixelShader("WaterModelPixelShader");
    // The engine's bulk asset creation treats static-mesh metadata as FBX input.
    // Register procedural geometry afterwards, using its public buffer factory.
    if (!Assets().HasMeshData("WaterPlane")) RegisterMeshes();
}
