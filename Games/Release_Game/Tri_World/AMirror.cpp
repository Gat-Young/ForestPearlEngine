#include "AMirror.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPSceneCaptureComponent2D.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/Systems/KeyStateEnum.h"

void AMirror::Initialize()
{
	MirrorPlane = new FPStaticMeshComponent(this, "Plane_StaticMesh");
	MirrorPlane->SetupAttachment(RootComponent);
	MirrorPlane->SetRelativeRotation({-90.0f, 0.0f, 0.0f});

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	MirrorPlane->SetMaterial(AssetManager->MakeMaterial("Mirror_Material"));

	//Player의 카메라 리그(SpringArm)와 동일한 오프셋/각도로 구성해 같은 방향을 바라보게 함
	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 20.0f;
	SpringArm->SetRelativeLocation({ 0.0f, 1.2f, 0.0f });
	SpringArm->SetRelativeRotation({ 30.0f, 0.0f, 0.0f });

	//Possess된 동안 Controller의 회전을 그대로 받아 Player와 동일하게 카메라를 움직임
	SpringArm->bUsePawnControlRotation = true;

	ScreenCaptureComponent = new FPSceneCaptureComponent2D(this);
	ScreenCaptureComponent->SetupAttachment(SpringArm);

	std::string MirrorRenderTarget = "Mirror_RenderTarget";
	std::string MirrorDepthStencil = "Mirror_DepthStencil";
	ScreenCaptureComponent->SetRenderTarget(MirrorRenderTarget);
	ScreenCaptureComponent->SetDepthStencil(MirrorDepthStencil);

	//거울 자신의 Plane이 캡처에 함께 그려지지 않도록 HiddenActors에 등록
	ScreenCaptureComponent->AddHiddenActor(this);

	//Fov/Zn/Zf는 Player의 카메라와 동일한 기본값을 그대로 사용. Aspect만 512x512 RenderTarget에 맞춰 1.0으로 설정
	ScreenCaptureComponent->SetAspect(1.0f);

	//Player와 동일한 카메라 이동/줌 입력을 바인딩 (Possess되어 있을 때만 AddControllerYaw/PitchInput이 실제로 적용됨)
	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &AMirror::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_ZoomCamera", this, EKeyState::Pressed, &AMirror::CameraZoom);
}

void AMirror::BeginPlay()
{

}

void AMirror::Tick()
{
	__super::Tick();
}

void AMirror::CameraMove(FInputValue Value)
{
	float mov = 30.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}

void AMirror::CameraZoom(FInputValue Value)
{
	float mov = 30.0f;
	float mov_size = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	SpringArm->TargetArmLength = SpringArm->TargetArmLength + mov_size;
}