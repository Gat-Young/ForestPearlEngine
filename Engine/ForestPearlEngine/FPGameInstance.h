#pragma once
#include "Object/Object.h"
#include <string>
#include <memory>

class GameTimer;
class FPGameInstanceSubSystem;

enum class GameInstanceSubSystemName : size_t
{
	GameTimer,
	ClassRegister,
	InputSystem,
	AssetManager,
	AssetLoader,
	MeshRenderList,
	TextRenderList,
	GizmoRenderList,
	CameraList,
	GameProjectSetting,
	ViewPortClient,

	GameInstanceSubSystemList_MAX
};

class FPGameInstance : public FPObject
{
	private:
		FPGameInstance();
		virtual ~FPGameInstance() override;
		FPGameInstance(const FPGameInstance&) = delete;
		FPGameInstance& operator=(const FPGameInstance&) = delete;

		//////////////////////////////////////////
		// GamePlay시 하나만 존재해야하는 객체들

		//World
		struct WorldContext
		{
			std::string WorldName = "";
			std::unique_ptr<FPWorld> World = nullptr;
		};
		WorldContext GameWorld;

		//GameInstanceSubSystem이 들어있는 배열
		FPGameInstanceSubSystem* GameInstanceSubSystem[static_cast<size_t>(GameInstanceSubSystemName::GameInstanceSubSystemList_MAX)];

	public:
		static FPGameInstance& Get() { static FPGameInstance Instance; return Instance; };

		//레벨 전환
		void OpenLevel(std::string LevelName);

		//월드 반환
		FPWorld* GetWorld() override;

		//GameInstanceSubSystem 반환
		FPGameInstanceSubSystem* GetInstanceSubSystem(GameInstanceSubSystemName SubSystemName);
		FPGameInstanceSubSystem* GetClassRegister();
		FPGameInstanceSubSystem* GetGameTimer();
		FPGameInstanceSubSystem* GetAssetManager();
		FPGameInstanceSubSystem* GetAssetLoader();
		FPGameInstanceSubSystem* GetInputSystem();
		FPGameInstanceSubSystem* GetTextRenderList();
		FPGameInstanceSubSystem* GetCameraList();
		FPGameInstanceSubSystem* GetMeshRenderList();
		FPGameInstanceSubSystem* GetGameProjectSetting();
		FPGameInstanceSubSystem* GetViewPortClient();
		FPGameInstanceSubSystem* GetGizmoRenderList();
	

		void Initialize();
		void BeginPlay();
		void Tick();
		void UnLoadData();
		void Finalize();


};