#include "FPSkyLightComponent.h"
#include "FPGameInstance.h"

FPSkyLightComponent::FPSkyLightComponent(FPActor* Owner) : FPLightComponentBase(Owner)
{
	RegistSkyLightRenderList();
}

FPSkyLightComponent::~FPSkyLightComponent()
{
}

void FPSkyLightComponent::RegistSkyLightRenderList()
{
	//SkyLight Render List¿¡ µî·Ï
	FPSkyLightRenderList* SkyLightRenderList = static_cast<FPSkyLightRenderList*>(FPGameInstance::Get().GetSkyLightRenderList());
	RenderItem = SkyLightRenderList->RegistRenderList();

	if (RenderItem == nullptr) { return; }

	RenderItem->Active = &(this->isActive);
	RenderItem->LightIntensity = &(this->LightIntensity);
	RenderItem->LightColor = &(this->LightColor);
	RenderItem->CubeMap = &(this->CubeMap);
}

void FPSkyLightComponent::SetRealTimeCapture(bool RealTimeCapture)
{
	this->RealTimeCapture = RealTimeCapture;
}

void FPSkyLightComponent::SetLightIntnesity(float Intensity)
{
	this->LightIntensity = Intensity;
}

void FPSkyLightComponent::SetLightColor(FPVector4 Color)
{
	this->LightColor = Color;
}

void FPSkyLightComponent::SetCubeMap(std::string CubeMap)
{
	this->CubeMap = CubeMap;
}
