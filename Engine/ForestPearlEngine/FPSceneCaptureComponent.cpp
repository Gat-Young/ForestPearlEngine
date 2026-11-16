#include "FPSceneCaptureComponent.h"
#include "FPGameInstance.h"
#include "FPGameProjectSetting.h"

FPSceneCaptureComponent::FPSceneCaptureComponent(FPActor* Owner) : FPSceneComponent(Owner)
{
}

FPSceneCaptureComponent::~FPSceneCaptureComponent()
{
	FPSceneCaptureList* SceneCaptureList = static_cast<FPSceneCaptureList*>(FPGameInstance::Get().GetSceneCaptureList());
	SceneCaptureList->UnregistRenderList(SceneCaptureItem);
}
