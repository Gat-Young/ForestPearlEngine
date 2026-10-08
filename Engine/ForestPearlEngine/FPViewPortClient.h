#pragma once
#include "FPGameInstanceSubSystem.h"
#include <vector>

struct FPViewPort;

struct alignas(16) VertexConst
{
	float AniOn = 1.0f;
	float BlendOn = 1.0f;
};

enum class FPViewPortName : size_t
{
	MainGameViewPort,
	UIViewPort,
	TripleWaySplitViewPort,

	FPViewPort_MAX_SIZE
};

class FPViewPortClient : public FPGameInstanceSubSystem
{
	private:

		//ViewPort가 들어 있는 배열 (여러개인 경우 대응)
		std::vector<FPViewPort*> ViewPortArray[static_cast<size_t>(FPViewPortName::FPViewPort_MAX_SIZE)];

	public:
		FPViewPortClient();
		~FPViewPortClient() = default;

		std::vector<FPViewPort*> GetViewPort(FPViewPortName ViewPortName);

		void CreateViewPort();

		void CalculateViewPortSize(std::vector<FPViewPort*> ViewPort, int Width, int Heigth, float Aspect);

		void CalculateAllViewPortSize();

		void CalculateUIViewPortSize(std::vector<FPViewPort*> ViewPort, int Width, int Heigth, float Aspect);
};
