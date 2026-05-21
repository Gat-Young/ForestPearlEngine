#pragma once
#include <vector>
#include "../Object/Actor.h"

class Renderer
{
	public: 
		virtual void Rendering(std::vector<FPActor*> RenderList) = 0;
};

