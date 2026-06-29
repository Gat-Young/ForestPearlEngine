#pragma once
#include "Object/Object.h"
#include "Object/Actor.h"
#include "FPWorld.h"
#include <string>
#include <vector>

class FPLevel : public FPObject
{
	protected:
		std::vector<std::string> ActorlList;
		std::vector<FPActor*> GameActorList;

	public:
		FPLevel() = default;
		~FPLevel() = default;

		//상속해서 게임에 사용할 게임 오브젝트들을 만든다.
		virtual void Initialize() override;
		//상속 후 반드시 Super 할 것
		virtual void BeginPlay() override;
		virtual void Tick() override;

		//필요한 경우 상속해서 수행
		virtual void UnLoadData();
		virtual void Finalize();

		//월드 반환
		virtual FPWorld* GetWorld() override final { return Outer->GetWorld(); }

		//Actor List에 Actor를 등록
		void TryAddActorToList(FPActor* Actor);

		//ActorList 참조 전달
		std::vector<FPActor*>& GetGameActorList() { return GameActorList; }
};