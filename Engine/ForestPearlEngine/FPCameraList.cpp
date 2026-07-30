#include "FPCameraList.h"

CameraItem* FPCameraList::RegistRenderList()
{
	CamList.push_back(CameraItem{});

	return &(CamList.back());
}

void FPCameraList::UnregistRenderList(CameraItem* RenderItem)
{
	auto it = std::find_if(CamList.begin(), CamList.end(),
		[RenderItem](CameraItem& item)
		{
			return &item == RenderItem;
		});

	if (it != CamList.end())
	{
		CamList.erase(it);
	}
}