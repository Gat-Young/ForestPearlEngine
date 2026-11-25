#pragma once
#include "FPLightComponent.h"

class FPDirectionalLightComponent : public FPLightComponent
{
	protected:
        bool CastShadow = true;                     //해당 광원의 그림자 생성 여부
        unsigned int CascadeCount = 4;              //Cascade 개수
        unsigned int ShadowResolution = 2048;       //Cascade 해상도
        float DynamicShadowDistance = 150.0f;       //CSM 최대거리 f
        float CascadeSplitLambda = 0.75f;           //Uniform Split과 Log Split의 보간값 
        float CascadeTransitionFraction = 0.1f;     // Cascade 사이 보간 영역
        float ShadowDistanceFadeoutFraction = 0.1f; // 최대 그림자 거리에서 Fade Out
        float ShadowBias = 0.0005f;                 // Depth 오차 보정
        float ShadowSlopeBias = 0.0005f;            // Slope-Scaled Depth Bias
        float DistanceMargin = 200.0f;                 // Cascade Far 평면 거리 여유값
       
        std::string CascadeShadowMapName = "";

        FPCascadeShadowRenderItem* CascadeShadowRenderItem = nullptr;

        void RegistCascadeShadowRenderList();

	public:
		FPDirectionalLightComponent(FPActor* Owner);
		~FPDirectionalLightComponent();

		void SetCascadeCount(unsigned int InCascadeCount);
		void SetShadowResolution(unsigned int InShadowResolution);

		void SetDynamicShadowDistance(float InDynamicShadowDistance);
		void SetCascadeSplitLambda(float InCascadeSplitLambda);
		void SetCascadeTransitionFraction(float InCascadeTransitionFraction);
		void SetShadowDistanceFadeoutFraction(float InShadowDistanceFadeoutFraction);
		void SetShadowBias(float InShadowBias);
		void SetShadowSlopeBias(float InShadowSlopeBias);
		void SetDistanceMargin(float InDistanceMargin);
};
