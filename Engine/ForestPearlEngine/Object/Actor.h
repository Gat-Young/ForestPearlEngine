#pragma once
#include "Object.h"
#include <string>
#include "tchar.h"
#include <vector>
#include "../FPLevel.h"

class FPActor : public FPObject
{

protected:
	struct FPTransform
	{
		float x;
		float y;
		float z;
	};

	struct UIContext
	{
		int x;
		int y;
		unsigned long color;
		std::basic_string<TCHAR> msg;
	};

	public :
		FPTransform Transform;
		std::vector<UIContext*> UI_data;

		virtual void Initialize() override = 0;
		virtual void BeginPlay() override = 0;
		virtual void Tick() override = 0;

		virtual ~FPActor() = default;

		virtual FPWorld* GetWorld() override final { return Outer->GetWorld(); };
};