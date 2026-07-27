#pragma once
#include "Object/Object.h"
#include <string>
#include <memory>

class GameTimer;
class FPWorld;

class FPGameInstanceSubSystem;

class FPGameInstance : public FPObject
{
	private:
		FPGameInstance();
		~FPGameInstance() = default;

		FPGameInstance(const FPGameInstance&) = delete;
		FPGameInstance& operator=(const FPGameInstance&) = delete;

		// GamePlay시 하나만 존재해야하는 객체들

		//World
		struct WorldContext
		{
			std::string WorldName = "";
			std::unique_ptr<FPWorld> World = nullptr;
		};
		WorldContext GameWorld;

		//GameTimer
		FPGameInstanceSubSystem* Gametimer;

		//등록된 클래스 모음
		FPGameInstanceSubSystem* ClassRegister;

		//InputSystem
		FPGameInstanceSubSystem* InputSystem;

		//AssetManager
		FPGameInstanceSubSystem* AssetManager;

		//MeshRenderList
		FPGameInstanceSubSystem* MeshRenderList;

		//TextRenderList
		FPGameInstanceSubSystem* TextRenderList;

		//CameraList
		FPGameInstanceSubSystem* CameraList;


	public:
		//Single Tone
		static FPGameInstance& Get()
		{
			static FPGameInstance Instance;
			return Instance;
		}

		//레벨 전환
		void OpenLevel(std::string WorldName);

		//월드 반환
		FPWorld* GetWorld() override;

		GameTimer* GetGameTimer();

		void Initialize();
		void BeginPlay();
		void Tick();
		void UnLoadData();
		void Finalize();


};