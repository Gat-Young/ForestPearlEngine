#pragma once
#include "./Object/Object.h"
#include <vector>

class FPWorld : public FPObject
{
	private:
	public:
		FPWorld() = default;
		~FPWorld() = default;

		void LoadData();
		void BeginPlay() override;
		void Tick() override;
		void UnLoadData();
		void Finalize();
};