#include "CameraList.h"

CameraItem* CameraList::RegistRenderList()
{
	CamList.push_back(CameraItem{});

	return &(CamList.back());
}

void CameraList::UnregistRenderList(CameraItem* RenderItem)
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