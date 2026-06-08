#pragma once
#include "Object/Object.h"
#include <vector>

class FPLevel : public FPObject
{
	private:
	public:
		FPLevel() = default;
		~FPLevel() = default;

		virtual void LoadData() = 0;
		virtual void BeginPlay() = 0;
		virtual void Tick() = 0;
		virtual void UnLoadData() = 0;
		virtual void Finalize() = 0;
};