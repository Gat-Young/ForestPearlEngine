#pragma once
#include "GameProjectClassRegistry.h"

class FPGameInstance
{
	private:
		FPGameInstance() = default;
		~FPGameInstance() = default;

		FPGameInstance(const FPGameInstance&) = delete;
		FPGameInstance& operator=(const FPGameInstance&) = delete;

		struct WorldContext
		{
			std::string WorldName = "";
			std::unique_ptr<FPObject> World = nullptr;
		};

		WorldContext GameWorld;

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
		FPObject* GetWorld();

		void LoadData();
		void BeginPlay();
		void Tick();
		void UnLoadData();
		void Finalize();

};