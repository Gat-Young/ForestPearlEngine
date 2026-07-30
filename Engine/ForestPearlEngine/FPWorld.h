#pragma once
#include "./Object/Object.h"
#include "FPGameProjectClassRegistry.h"
#include "FPGameInstance.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class FPAGameMode;
class FPGameTimer;
class FPActor;

class FPWorld : public FPObject
{
	protected:
		std::vector<FPActor*> GameActorList;
		std::unique_ptr<FPAGameMode> GameMode;

	public:
		FPWorld();
		virtual ~FPWorld() override;

		//상속 후 Super를 반드시 할 것
		virtual void Initialize();
		virtual void BeginPlay();
		virtual void Tick();

		//월드 반환
		virtual FPWorld* GetWorld() override final { return this; };

		//필요한 경우 수행
		virtual void UnLoadData();
		virtual void Finalize();

		//레벨 전환 멤버 함수
		void OpenLevel(std::string LevelPath);

		//GameController 반환
		class FPAController* GetController(int index);

		//GameMode 반환
		FPAGameMode* GetAuthGameMode();

		//GameTimer 반환
		FPGameTimer* GetGameTimer();

		//Actor 생성
		FPActor* SpawnActor(std::string ActorClassName, std::string ActorName = "");

		//ActorList 참조 전달
		std::vector<FPActor*>& GetGameActorList();

		//Class Instance 생성 템플릿 함수
		template <typename T>
		T* CreateClassInstnce(std::string ClassName)
		{
			FPGameInstance* GameInstance = static_cast<FPGameInstance*>(GetOuter());

			FPGameProjectClassRegistry* ClassRegistry = static_cast<FPGameProjectClassRegistry*>(GameInstance->GetClassRegister());

			if (!ClassRegistry->HasFactory(ClassName)) { return nullptr; }

			std::unique_ptr<FPObject> ClassObject = ClassRegistry->Create(ClassName);

			T* ClassInstance = dynamic_cast<T*>(ClassObject.get());

			ClassObject.release();

			return ClassInstance;
		}
};