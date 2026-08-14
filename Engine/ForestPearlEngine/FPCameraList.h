#pragma once
#include <vector>
#include "../ForestPearlEngine/Define/FPMath.h"
#include "FPGameInstanceSubSystem.h"
#include "../ForestPearlEngine/Renderers/FPRenderingCommon.h"

class FPCameraList : public FPGameInstanceSubSystem
{
	private:
		std::vector<CameraItem> CamList;

	public:

		FPCameraList() = default;
		~FPCameraList() = default;

		CameraItem* RegistRenderList();
		void UnregistRenderList(CameraItem* RenderItem);

		std::vector<CameraItem>& GetRenderList()
		{
			return CamList;
		}
};