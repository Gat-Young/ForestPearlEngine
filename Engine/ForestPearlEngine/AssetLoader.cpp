#include "FPAssetLoader.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <fstream>
#include <iostream>
#include "Libraries/nlohmann/json.hpp"
#include "Utility/FPPathManager.h"
#include "Renderers/RenderingDevice.h"

using json = nlohmann::json;

// 색상 JSON 읽기
COLOR_VERTEX ReadColor(const json& ColorJson)
{
	COLOR_VERTEX Color;

	Color.r = ColorJson.value("r", 1.0f);
	Color.g = ColorJson.value("g", 1.0f);
	Color.b = ColorJson.value("b", 1.0f);
	Color.a = ColorJson.value("a", 1.0f);

	return Color;
}

//GameSetting Data를 저장
void FPAssetLoader::LoadGameSetting(std::string GameSettingPath)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	std::string Path = FPPathManager::Get().GetAssetPath("GameSetting/" + GameSettingPath);
	std::ifstream File(Path);

	if (!File.is_open())
	{
		std::cerr << "GameSetting 파일 열기 실패: " << Path << '\n';
	}

	try
	{
		FPGameSettingData OutSetting;

		json JsonData;
		File >> JsonData;

		const auto& GameSetting = JsonData.at("GameSetting");
		const auto& Graphics = GameSetting.at("Graphics");
		const auto& Resolution = Graphics.at("Resolution");

		// 게임 이름
		OutSetting.GameName = GameSetting.at("GameName").get<std::string>();

		// 그래픽 옵션
		OutSetting.MSAA = Graphics.at("MSAA").get<int>();
		if ((OutSetting.MSAA % 2 != 0) || OutSetting.MSAA > 8)
		{
			std::cout << "MSAA를 1로 설정 합니다." << "\n";
			OutSetting.MSAA = 1;
		}

		OutSetting.AnisotropicFiltering = Graphics.at("AnisotropicFiltering").get<int>();
		if ((OutSetting.AnisotropicFiltering % 2 != 0) || OutSetting.AnisotropicFiltering > 16)
		{
			std::cout << "AnisotropicFiltering을 1로 설정합니다." << "\n";
			OutSetting.AnisotropicFiltering = 1;
		}

		// 해상도
		OutSetting.Width = Resolution.at("Width").get<int>();
		OutSetting.Height = Resolution.at("Height").get<int>();

		// 전체 화면
		OutSetting.FullScreen = Graphics.at("FullScreen").get<bool>();

		AssetManager->UpdateGameSettingData(OutSetting);
	}
	catch (const json::exception& e)
	{
		std::cerr << "GameSetting JSON 읽기 실패: " << e.what() << '\n';
	}
}

//Level 정보를 저장
void FPAssetLoader::LoadLevelData(std::string LevelName, std::string LevelPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	if (AssetManager->HasLevelData(LevelName))
	{
		std::cout << "같은 이름의 Level이 존재합니다." << "\n";
		return;
	}

	std::ifstream File(FPPathManager::Get().GetAssetPath("Level/" + LevelPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Level/" + LevelPath));

	if (!File.is_open())
	{
		std::cerr << LevelName << ".json 파일 열기 실패\n";
		return;
	}

	json JsonLevelData;

	try
	{
		File >> JsonLevelData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	std::string GameModeName = JsonLevelData["gamemode"];
	AssetManager->AddGameModeData(LevelName, GameModeName);

	for (const json& ActorData : JsonLevelData["actors"])
	{
		std::string ClassName = ActorData["class"];
		std::string ActorName = ActorData["name"];

		const json& Transform = ActorData["transform"];
		const json& Location = Transform["location"];
		const json& Rotation = Transform["rotation"];
		const json& Scale = Transform["scale"];

		float LocationX = Location["x"];
		float LocationY = Location["y"];
		float LocationZ = Location["z"];

		float RotationX = Rotation["x"];
		float RotationY = Rotation["y"];
		float RotationZ = Rotation["z"];

		float ScaleX = Scale["x"];
		float ScaleY = Scale["y"];
		float ScaleZ = Scale["z"];

		FPActorData Data;

		Data.ClassName = ClassName;
		Data.ActorName = ActorName;

		Data.Location_x = LocationX;
		Data.Location_y = LocationY;
		Data.Location_z = LocationZ;

		Data.Rotation_x = RotationX;
		Data.Rotation_y = RotationY;
		Data.Rotation_z = RotationZ;

		Data.Scale_x = ScaleX;
		Data.Scale_y = ScaleY;
		Data.Scale_Z = ScaleZ;

		AssetManager->AddLevelData(LevelName, &Data);
	}
}

//VertexShader
void FPAssetLoader::LoadVertexShader(std::string ShaderPath, AssetOwner EngineAsset)
{
	std::ifstream File(FPPathManager::Get().GetAssetPath("Shader/" + ShaderPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Shader/" + ShaderPath));

	if (!File.is_open())
	{
		std::cerr << ShaderPath << ".json 파일 열기 실패\n";
		return;
	}

	json ShaderData;

	try
	{
		File >> ShaderData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	FPVertexShaderData VertexShaderData;
	VertexShaderData.Name = ShaderData.at("Name").get<std::string>();
	VertexShaderData.Path = ShaderData.at("Path").get<std::string>();
	VertexShaderData.InputLayout = ShaderData.at("InputLayout").get<std::string>();
	VertexShaderData.Owner = EngineAsset;

	AssetManager->AddVertexShaderData(VertexShaderData.Name, VertexShaderData);
}

//PixelShader
void FPAssetLoader::LoadPixelShader(std::string ShaderPath, AssetOwner EngineAsset)
{
	std::ifstream File(FPPathManager::Get().GetAssetPath("Shader/" + ShaderPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Shader/" + ShaderPath));

	if (!File.is_open())
	{
		std::cerr << ShaderPath << ".json 파일 열기 실패\n";
		return;
	}

	json ShaderData;

	try
	{
		File >> ShaderData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	FPPixelShaderData PixelShaderData;
	PixelShaderData.Name = ShaderData.at("Name").get<std::string>();
	PixelShaderData.Path = ShaderData.at("Path").get<std::string>();
	PixelShaderData.Owner = EngineAsset;

	AssetManager->AddPixelShaderData(PixelShaderData.Name, PixelShaderData);
}


//StaticMesh
void FPAssetLoader::LoadStaticMesh(std::string StaticMeshPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	std::ifstream File(FPPathManager::Get().GetAssetPath("StaticMesh/" + StaticMeshPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("StaticMesh/" + StaticMeshPath));

	if (!File.is_open())
	{
		std::cerr << StaticMeshPath << ".json 파일 열기 실패\n";
		return;
	}

	json JsonStaticMeshData;

	try
	{
		File >> JsonStaticMeshData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPStaticMeshData Data;
	Data.Name = JsonStaticMeshData.at("Name").get<std::string>();
	Data.NodeName = JsonStaticMeshData.at("NodeName").get<std::string>();

	if (AssetManager->HasStaticMeshData(Data.Name))
	{
		std::cout << "같은 이름의 StaticMesh가 존재합니다." << "\n";
		return;
	}

	// MeshData 배열
	if (JsonStaticMeshData.contains("MeshData") && JsonStaticMeshData.at("MeshData").is_array())
	{
		for (const auto& Mesh : JsonStaticMeshData.at("MeshData"))
		{
			Data.MeshPath = Mesh.at("Path").get<std::string>();

			Data.MeshTopology = StringToTopology(Mesh.at("Topology").get<std::string>());
		}
	}

	if (JsonStaticMeshData.contains("Sockets") && JsonStaticMeshData["Sockets"].is_array())
	{
		for (const json& SocketData : JsonStaticMeshData["Sockets"])
		{
			std::string SocketName = SocketData["Name"];

			const json& Transform = SocketData["Transform"];
			const json& Location = Transform["Location"];
			const json& Rotation = Transform["Rotation"];
			const json& Scale = Transform["Scale"];

			float LocationX = Location["x"];
			float LocationY = Location["y"];
			float LocationZ = Location["z"];

			float RotationX = Rotation["x"];
			float RotationY = Rotation["y"];
			float RotationZ = Rotation["z"];

			float ScaleX = Scale["x"];
			float ScaleY = Scale["y"];
			float ScaleZ = Scale["z"];

			SOCKET_TRANSFORM SocketTransform;
			SocketTransform.SocketName = SocketName;
			SocketTransform.Location_x = LocationX;
			SocketTransform.Location_y = LocationY;
			SocketTransform.Location_z = LocationZ;

			SocketTransform.Rotation_x = RotationX;
			SocketTransform.Rotation_y = RotationY;
			SocketTransform.Rotation_z = RotationZ;

			SocketTransform.Scale_x = ScaleX;
			SocketTransform.Scale_y = ScaleY;
			SocketTransform.Scale_Z = ScaleZ;

			Data.Sockets.push_back(SocketTransform);
		}
	}


	//Material 값 일단 Maapping해서 들고 있기
	if (JsonStaticMeshData.contains("Material") && JsonStaticMeshData.at("Material").is_object())
	{
		const json& Material = JsonStaticMeshData.at("Material");

		Data.MaterialName = Material.value("Name", std::string{});
	}

	Data.Owner = EngineAsset;

	AssetManager->AddStaticMeshData(Data.Name, &Data);
}

//Texture
void FPAssetLoader::LoadTexture(std::string TexturePath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	std::ifstream File(FPPathManager::Get().GetAssetPath("Texture/" + TexturePath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Texture/" + TexturePath));

	if (!File.is_open())
	{
		std::cerr << TexturePath << "파일 열기 실패\n";
		return;
	}

	json JsonTextureData;

	try
	{
		File >> JsonTextureData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPTextureInfo TextureInfo;

	TextureInfo.Name = JsonTextureData["Name"].get<std::string>();
	TextureInfo.Path = JsonTextureData["Path"].get<std::string>();
	TextureInfo.Type = JsonTextureData["Type"].get<std::string>();
	TextureInfo.Owner = EngineAsset;

	if (AssetManager->HasTextureData(TextureInfo.Name))
	{
		std::cout << "같은 이름의 Texture가 존재합니다." << "\n";
		return;
	}

	//AssetManager에 Load한 정보 등록
	AssetManager->AddTextureInfoData(TextureInfo.Name, TextureInfo);
}

//Material Data Loader
void FPAssetLoader::LoadMaterial(std::string MaterialPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	std::ifstream File(FPPathManager::Get().GetAssetPath("Material/" + MaterialPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Material/" + MaterialPath));

	if (!File.is_open())
	{
		std::cerr << MaterialPath << "파일 열기 실패\n";
		return;
	}

	json JsonMaterialData;

	try
	{
		File >> JsonMaterialData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPMaterialData MaterialData;

	// Material 객체
	const json& Material = JsonMaterialData.at("Material");

	MaterialData.Name = Material.at("Name").get<std::string>();

	MaterialData.VertexShader = Material.at("VertexShader").get<std::string>();

	MaterialData.PixelShader = Material.at("PixelShader").get<std::string>();

	// Color
	MaterialData.Ambient = ReadColor(Material.at("Ambient"));

	MaterialData.BaseColor = ReadColor(Material.at("BaseColor"));

	MaterialData.Diffuse = ReadColor(Material.at("Diffuse"));

	MaterialData.Emissive = ReadColor(Material.at("Emissive"));

	//Two Side
	MaterialData.TwoSided = Material.value("TwoSided",false);

	// Texture 배열
	if (JsonMaterialData.contains("Texture") && JsonMaterialData.at("Texture").is_array())
	{
		for (const auto& Texture : JsonMaterialData.at("Texture"))
		{
			std::string Name = Texture.at("Name").get<std::string>();
			unsigned int Slot = Texture.at("Slot").get<unsigned int>();
			std::string TextureFilter = Texture.at("TextureFilter").get<std::string>();
			std::string Sampler = Texture.at("SamplerType").get<std::string>();
			FPSamplerType SamplerType = StringToSamplerType(TextureFilter + "_" + Sampler);
			std::string TypeString = Texture.at("Type").get<std::string>();
			FPTextureType TextureType = StringToTextureType(TypeString);

			MaterialData.Textures.push_back({Slot, Name, TextureType, SamplerType});
		}
	}
	MaterialData.Owner = EngineAsset;

	AssetManager->AddMaterialData(MaterialData.Name, MaterialData);
}
