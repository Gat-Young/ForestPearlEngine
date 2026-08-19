#pragma once
#include "FPGameInstanceSubSystem.h"
#include <vector>

struct FPViewPort
{
	float TopLeftX = 0.0f;
	float TopLeftY = 0.0f;
	float Width;
	float Height;
	float MinDepth = 0.0f;
	float MaxDepth = 1.0f;
	//상수 버퍼용
	void* VertexConst = nullptr;
	void* PixelConst = nullptr;
};

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

		void CalculateViewPortSize(std::vector<FPViewPort*> ViewPort, int Width, int Heigth, float Aspect);
};
