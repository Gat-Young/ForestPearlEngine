#pragma once
#include "./Object/Object.h"
#include "FPLevel.h"
#include <memory>
#include <vector>

class FPWorld : public FPObject
{
	private:
		//0¹ø Index´Â PrimitiveLevel
		std::vector<std::unique_ptr<FPLevel>> Levels;

	public:
		FPWorld() = default;
		~FPWorld() = default;

		virtual void LoadData() = 0;
		virtual void BeginPlay() override;
		virtual void Tick() override;
		virtual void UnLoadData() = 0;
		virtual void Finalize() = 0;
};