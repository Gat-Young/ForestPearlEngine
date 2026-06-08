#pragma once
#include "Object/Object.h"
#include <vector>

class FPLevel : public FPObject
{
	private:
	public:
		FPLevel() = default;
		~FPLevel() = default;

		void LoadData();
		void BeginPlay() override;
		void Tick() override;
		void UnLoadData();
		void Finalize();
};