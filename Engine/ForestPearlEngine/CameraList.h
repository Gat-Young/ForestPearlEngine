#pragma once
#include <vector>
#include "../ForestPearlEngine/Define/FPMath.h"

struct CameraItem
{
	//카메라의 위치
	FPVector3* Position = nullptr;
	FPVector3* Rotation = nullptr;
	FPVector3* Scale = nullptr;

	//카메라 속성
	FPVector3* LookAt = nullptr;		//바라보는 곳. 위치
	FPVector3* Up = nullptr;			//카메라 상방 벡터

	//투영 변환 용
	float* Fov = nullptr;				//시야각			
	float* Aspect = nullptr;			//가로:세로 비율
	float* Zn = nullptr;				//근평면 거리
	float* Zf = nullptr;				//원평면 거리

	bool* Active = nullptr;			//카메라 사용 여부
};

class CameraList
{
	private:
		std::vector<CameraItem> CamList;

		CameraList() = default;
		~CameraList() = default;

	public:
		//Single Tone
		static CameraList& Get()
		{
			static CameraList Instance;
			return Instance;
		}

		CameraItem* RegistRenderList();
		void UnregistRenderList(CameraItem* RenderItem);

		std::vector<CameraItem>& GetRenderList()
		{
			return CamList;
		}
};