#include "Windmill.h"
#include "ForestPearlEngine/FPMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"

void Windmill::Initialize()
{
	Body = new FPMeshComponent(this, "Windmill/Windmill_Body.fbx");
	SetRootComponent((FPSceneComponent*)Body);
	Body->SetMeshCull(false);

	Wing = new FPMeshComponent(this, "Windmill/Windmill_Wing.fbx");
	Wing->SetMeshCull(false);
	Wing->SetupAttachment(Body);
	Wing->SetRelativeLocation({0.0f, 3.0f, -1.0f});
	Wing->SetRelativeRotation({-90.0f, 0.0f, 0.0f});

	std::cout << "Body Rotation : " << Body->GetComponentRotation().x << " : " << Body->GetComponentRotation().y << " : " << Body->GetComponentRotation().z << "\n";
	std::cout << "Body Rotation : " << Body->GetRelativeRotation().x << " : " << Body->GetRelativeRotation().y << " : " << Body->GetRelativeRotation().z << "\n";

	std::cout << "Wing Rotation : " << Wing->GetComponentRotation().x << " : " << Wing->GetComponentRotation().y << " : " << Wing->GetComponentRotation().z << "\n";
	std::cout << "Wing Rotation : " << Wing->GetRelativeRotation().x << " : " << Wing->GetRelativeRotation().y << " : " << Wing->GetRelativeRotation().z << "\n";
 
	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetFillTriangel", this, EKeyState::Down, &Windmill::SetFillTriangel);
	Controller->GetInputComponent().BindMethod("IA_SetCullTriangel", this, EKeyState::Down, &Windmill::SetCullTriangle);
}

void Windmill::BeginPlay()
{
}

void Windmill::Tick()
{
	FPGameTimer* GameTimer = static_cast<FPGameTimer*>(FPGameInstance::Get().GetGameTimer());

	float RotateSpeed = 180.0f;
	Wing->AddLocalRotation({ 0.0f,  RotateSpeed * GameTimer->DeltaTime(),0.0f});
	__super::Tick();
}

void Windmill::SetFillTriangel(FInputValue Value)
{
	isFill = !isFill;
	Body->SetMeshFill(isFill);
	Wing->SetMeshFill(isFill);
}

void Windmill::SetCullTriangle(FInputValue Value)
{
	isCull = !isCull;
	Body->SetMeshCull(isCull);
	Wing->SetMeshCull(isCull);
}