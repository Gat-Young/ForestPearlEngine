#include "FPAssetLoader.h"
#include "FPGameInstance.h"
#include "FPAssetManager.h"
#include <fstream>
#include <iostream>
#include <set>
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

void FPAssetLoader::LoadTexture2DArray(std::string Texture2DArrayPath, AssetOwner EngineAsset)
{
	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());

	std::ifstream File(FPPathManager::Get().GetAssetPath("Texture2DArray/" + Texture2DArrayPath));
	if (EngineAsset == AssetOwner::Engine) File = std::ifstream(FPPathManager::Get().GetEngineAssetPath("Texture2DArray/" + Texture2DArrayPath));

	if (!File.is_open())
	{
		std::cerr << Texture2DArrayPath << "파일 열기 실패\n";
		return;
	}

	json JsonTextureArrayData;

	try
	{
		File >> JsonTextureArrayData;
	}
	catch (const json::parse_error& Error)
	{
		std::cerr << "JSON 파싱 실패: "
			<< Error.what()
			<< '\n';

		return;
	}

	FPTextureArrayInfo TextureArrayInfo;

	TextureArrayInfo.Name = JsonTextureArrayData["Name"].get<std::string>();
	TextureArrayInfo.Width = JsonTextureArrayData["Width"].get<unsigned int>();
	TextureArrayInfo.Height = JsonTextureArrayData["Height"].get<unsigned int>();
	TextureArrayInfo.ArraySize = JsonTextureArrayData["ArraySize"].get<unsigned int>();

	for (const auto& SourceTexture : JsonTextureArrayData["SourceTextures"])
	{
		TextureArrayInfo.SourceTextures.push_back(SourceTexture.get<std::string>());
	}

	TextureArrayInfo.Owner = EngineAsset;

	if (AssetManager->HasTextureArrayInfoData(TextureArrayInfo.Name))
	{
		std::cout << "같은 이름의 Texture2DArray가 존재합니다." << "\n";
		return;
	}

	//AssetManager에 Load한 정보 등록
	AssetManager->AddTextureArrayInfoData(TextureArrayInfo.Name, TextureArrayInfo);
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

	//Two Side
	MaterialData.TwoSided = Material.value("TwoSided", false);


	//Material Parameter 정보 가져오기
	const json& Groups = Material.at("Parameters");

	for (const json& Group : Groups)
	{
		//VS, PS 인지 구분
		const FPShaderType Stage = StringToFPShaderType(Group.at("Type").get<std::string>());
		if (Stage == FPShaderType::None) { std::cout << "Material의 Type은 VS or PS여야 합니다!" << "\n"; }

		//상수 버퍼이름 가져오기
		const std::string CB = Group.at("ConstantBuffer").get<std::string>();

		//상수 버퍼에 있는 실수 데이터(float, float2, float3, float4) 내부 요소 가져오기
		const json Values = Group.at("Values");

		for (json::const_iterator It = Values.begin(); It != Values.end(); ++It)
		{
			FPFloatParameterData Entry;
			Entry.Type = Stage;
			Entry.ConstantBufferName = CB;
			Entry.ParameterName = It.key();

			const json& Value = It.value();

			if (Value.is_number())		// float 타입 처리
			{
				Entry.Values.push_back(Value.get<float>());
			}
			else if (Value.is_array())	// float2, float3, float4 처리, float[] ,float2[], float3[], float4[] 처리
			{
				for (const json& X : Value)
				{
					Entry.Values.push_back(X.get<float>());
				}
			}

			MaterialData.Parameters.push_back(std::move(Entry));
		}
	}

	// Texture 처리
	if (Material.contains("Textures"))
	{
		const json& Textures = Material.at("Textures");

		for (const json& T : Textures)
		{
			FPTextureParameterData Entry;
			Entry.Type = StringToFPShaderType(T.at("Type").get<std::string>());
			Entry.Parameter = T.at("Parameter").get<std::string>();
			Entry.Name = T.at("Asset").get<std::string>();

			const std::string TextureType = T.at("TextureType").get<std::string>();
			if (TextureType != "Texture2D" && TextureType != "Texture2DArray") { std::cout << "Texture2D, Texture2DArray외의 Type이 사용됨"; }

			Entry.TextureType = StringToTextureType(TextureType);

			//Texture 배열에 대한 처리
			json::const_iterator It = T.find("ArrayIndex");
			if (It == T.end()) Entry.ArrayIndex = 0;
			else
			{
				const uint64_t Raw = It->get<uint64_t>();
				Entry.ArrayIndex = static_cast<unsigned int>(Raw);
			}

			MaterialData.NamedTextures.push_back(std::move(Entry));
		}
	}

	// Sampler 처리
	if (Material.contains("Samplers"))
	{
		const json& Samplers = Material.at("Samplers");

		for (const json& S : Samplers)
		{
			FPSamplerParameterData Entry;
			Entry.Type = StringToFPShaderType(S.at("Type").get<std::string>());
			Entry.Parameter = S.at("Parameter").get<std::string>();

			//Sampler 배열에 대한 처리
			json::const_iterator It = S.find("ArrayIndex");
			if (It == S.end()) Entry.ArrayIndex = 0;
			else
			{
				const uint64_t Raw = It->get<uint64_t>();
				Entry.ArrayIndex = static_cast<unsigned int>(Raw);
			}

			const std::string SamplerType = S.at("SamplerType").get<std::string>();
			Entry.SamplerType = StringToSamplerType(SamplerType);
			MaterialData.NamedSamplers.push_back(std::move(Entry));
		}
	}

	MaterialData.Owner = EngineAsset;

	AssetManager->AddMaterialData(MaterialData.Name, MaterialData);
}
