#include "FPGameInstance.h"
#include "FPWorld.h"
#include "FPGameTimer.h"
#include "FPGameProjectClassRegistry.h"
#include "Systems/InputSystem.h"
#include "FPAssetManager.h"
#include "FPMeshRenderList.h"
#include "FPTextRenderList.h"
#include "FPCameraList.h"
#include "FPAssetLoader.h"
#include "FPGameProjectSetting.h"
#include "FPViewPortClient.h"
#include "FPGizmoRenderList.h"
#include "FPLightRenderList.h"


auto Cast_SizeT = [](GameInstanceSubSystemName Name) -> size_t {return static_cast<size_t>(Name); };

FPGameInstance::FPGameInstance()
{
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::GameTimer)] = new FPGameTimer();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::ClassRegister)] = new FPGameProjectClassRegistry();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::InputSystem)] = new FPInputSystem();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::AssetManager)] = new FPAssetManager();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::AssetLoader)] = new FPAssetLoader();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::MeshRenderList)] = new FPMeshRenderList();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::TextRenderList)] = new FPTextRenderList();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::GizmoRenderList)] = new FPGizmoRenderList();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::LightRenderList)] = new FPLightRenderList();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::CameraList)] = new FPCameraList();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::GameProjectSetting)] = new FPGameProjectSetting();
	GameInstanceSubSystem[Cast_SizeT(GameInstanceSubSystemName::ViewPortClient)] = new FPViewPortClient();

}

FPGameInstance::~FPGameInstance() = default;

//Level을 열고 생성
void FPGameInstance::OpenLevel(std::string LevelName)
{
	if (GameWorld.World != nullptr) GameWorld.World.reset();

	GameWorld.WorldName = LevelName;

	GameWorld.World = std::make_unique<FPWorld>();

	GameWorld.World->SetOuter(this);

	GameWorld.World->OpenLevel(LevelName);
}

FPWorld* FPGameInstance::GetWorld()
{
	return GameWorld.World.get();
}

void FPGameInstance::Initialize()
{
	GameWorld.World->Initialize();
}

void FPGameInstance::BeginPlay()
{
	GameWorld.World->BeginPlay();
}

void FPGameInstance::Tick()
{
	static_cast<FPInputSystem*>(GetInputSystem())->TickInputSystem();
	static_cast<FPGameTimer*>(GetGameTimer())->Tick();
	GameWorld.World->Tick();
}

void FPGameInstance::UnLoadData()
{
	GameWorld.World->UnLoadData();
}

void FPGameInstance::Finalize()
{
	GameWorld.World->Finalize();
}

FPGameInstanceSubSystem* FPGameInstance::GetInstanceSubSystem(GameInstanceSubSystemName SubSystemName)
{
	return GameInstanceSubSystem[Cast_SizeT(SubSystemName)];
}

FPGameInstanceSubSystem* FPGameInstance::GetClassRegister()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::ClassRegister);
}

FPGameInstanceSubSystem* FPGameInstance::GetGameTimer()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::GameTimer);
}

FPGameInstanceSubSystem* FPGameInstance::GetAssetManager()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::AssetManager);
}

FPGameInstanceSubSystem* FPGameInstance::GetAssetLoader()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::AssetLoader);
}

FPGameInstanceSubSystem* FPGameInstance::GetInputSystem()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::InputSystem);
}

FPGameInstanceSubSystem* FPGameInstance::GetTextRenderList()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::TextRenderList);
}

FPGameInstanceSubSystem* FPGameInstance::GetCameraList()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::CameraList);
}

FPGameInstanceSubSystem* FPGameInstance::GetMeshRenderList()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::MeshRenderList);
}

FPGameInstanceSubSystem* FPGameInstance::GetGameProjectSetting()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::GameProjectSetting);
}

FPGameInstanceSubSystem* FPGameInstance::GetViewPortClient()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::ViewPortClient);
}

FPGameInstanceSubSystem* FPGameInstance::GetGizmoRenderList()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::GizmoRenderList);
}

FPGameInstanceSubSystem* FPGameInstance::GetLightRenderList()
{
	return GetInstanceSubSystem(GameInstanceSubSystemName::LightRenderList);
}
