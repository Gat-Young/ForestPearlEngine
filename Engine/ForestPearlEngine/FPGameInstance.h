#pragma once
#include "GameProjectClassRegistry.h"
#include "GameTimer.h"
#include "FPWorld.h"

class FPGameInstance
{
	private:
		FPGameInstance() { Gametimer = new GameTimer; };
		~FPGameInstance() = default;

		FPGameInstance(const FPGameInstance&) = delete;
		FPGameInstance& operator=(const FPGameInstance&) = delete;

		struct WorldContext
		{
			std::string WorldName = "";
			std::unique_ptr<FPWorld> World = nullptr;
		};

		WorldContext GameWorld;

		GameTimer* Gametimer;

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
		FPWorld* GetWorld();

		GameTimer* GetGameTimer() { return Gametimer; }

		void Initialize();
		void BeginPlay();
		void Tick();
		void UnLoadData();
		void Finalize();


};