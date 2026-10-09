#pragma once
#include "FPGizmoComponent.h"


struct FPGIZMO_NORMALLINEINFO {
	float length;		//각 방향축의 길이.
	float scale;		//비례 스케일
	float r, g, b, a;
	int scnX, scnY;		//화면 크기(Viewport 미사용시 적용)
	unsigned long res[20];

	FPGIZMO_NORMALLINEINFO(float len = 1.0f, float s = 1.0f) :length(len), scale(s), scnX(0), scnY(0) {}
};

class FPGizmoNormalLineComponent: public FPGizmoComponent
{
public:
	FPGizmoNormalLineComponent(FPActor* Owner, std::string& StaticMeshPath);
	~FPGizmoNormalLineComponent();
	void MakeNormalLine(const std::string& StaticMeshPath, FPGIZMO_NORMALLINEINFO& Line);

	void SetGizmoMeshPath(const std::string& StaticMeshPath) override;
};