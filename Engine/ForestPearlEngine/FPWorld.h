#pragma once
#include "./Object/Object.h"
#include "FPLevel.h"
#include "GameProjectClassRegistry.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class FPLevel;
class FPWorld : public FPObject
{
	protected:
		//Level List, 0번 Index는 PersistentLevel
		std::vector<std::string> LevelList;

		std::unique_ptr<FPLevel> PersistentLevel;
		std::unordered_map<std::string, std::unique_ptr<FPLevel>> StreamingLevel;

	public:
		FPWorld() = default;
		~FPWorld() = default;

		//상속 후 Super를 반드시 할 것
		virtual void Initialize() override;
		virtual void BeginPlay() override;
		virtual void Tick() override;

		//월드 반환
		virtual FPWorld* GetWorld() override final { return this; };

		//필요한 경우 수행
		virtual void UnLoadData(std::string LevelName);
		virtual void Finalize();

		//레벨 전환 멤버 함수
		void OpenLevel(std::string LevelName);


		//Class Instance 생성 템플릿 함수
		template <typename T>
		T* CreateClassInstnce(std::string ClassName)
		{
			if (!GameProjectClassRegistry::Get().HasFactory(ClassName)) { return nullptr; }

			std::unique_ptr<FPObject> ClassObject = GameProjectClassRegistry::Get().Create(ClassName);

			T* ClassInstance = dynamic_cast<T*>(ClassObject.get());

			ClassObject.release();

			return ClassInstance;
		}
};