#include "Player.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include "WindmillWing.h"
#include "TripleWindmillWing.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ATorchLight.h"
#include <iostream>

void Player::Initialize()
{

	//real Model
	
	
	BodyAndHat = new FPStaticMeshComponent(this, "ToonLink_BodyAndHat_StaticMesh");
	BodyAndHat->SetupAttachment(RootComponent);
	
	
	Buckle = new FPStaticMeshComponent(this, "ToonLink_Buckle_StaticMesh");
	Buckle->SetupAttachment(RootComponent);

	
	Eye = new FPStaticMeshComponent(this, "ToonLink_Eye_StaticMesh");
	Eye->SetupAttachment(RootComponent);

	EyeBrow = new FPStaticMeshComponent(this, "ToonLink_EyeBrow1_StaticMesh");
	EyeBrow->SetupAttachment(RootComponent);

	Face = new FPStaticMeshComponent(this, "ToonLink_Face_StaticMesh");
	Face->SetupAttachment(RootComponent);

	Hair = new FPStaticMeshComponent(this, "ToonLink_Hair_StaticMesh");
	Hair->SetupAttachment(RootComponent);

	Hand = new FPStaticMeshComponent(this, "ToonLink_Hand_StaticMesh");
	Hand->SetupAttachment(RootComponent);

	LeftEyeBg = new FPStaticMeshComponent(this, "ToonLink_LeftEyeBackground_StaticMesh");
	LeftEyeBg->SetupAttachment(RootComponent);

	RightEyebg = new FPStaticMeshComponent(this, "ToonLink_RightEyeBackground_StaticMesh");
	RightEyebg->SetupAttachment(RootComponent);

	MasterSword_Scabbard = new FPStaticMeshComponent(this, "ToonLink_MasterSword_Scabbard_StaticMesh");
	MasterSword_Scabbard->SetupAttachment(RootComponent);

	MasterSword_Handle = new FPStaticMeshComponent(this, "ToonLink_MasterSword_Handle_StaticMesh");
	MasterSword_Handle->SetupAttachment(RootComponent);

	MasterSword_Blade = new FPStaticMeshComponent(this, "ToonLink_MasterSword_Blade_StaticMesh");
	MasterSword_Blade->SetupAttachment(RootComponent);

	MasterSword_BackHandle = new FPStaticMeshComponent(this, "ToonLink_MasterSword_BackHandle_StaticMesh");
	MasterSword_BackHandle->SetupAttachment(RootComponent);

	Mouth = new FPStaticMeshComponent(this, "ToonLink_Mouse1_StaticMesh");
	Mouth->SetupAttachment(RootComponent);

	Shield_Back = new FPStaticMeshComponent(this, "ToonLink_Shield_Back_StaticMesh");
	Shield_Back->SetupAttachment(RootComponent);

	Shidld_Hand = new FPStaticMeshComponent(this, "ToonLink_Shield_Handle_StaticMesh");
	Shidld_Hand->SetupAttachment(RootComponent);
	

	ShieldPivot = new FPSceneComponent(this);
	ShieldPivot->SetupAttachment(RootComponent);
	ShieldPivot->SetRelativeLocation({ 0.0f, 2.0f, 0.0f });
	ShieldPivot->SetRelativeRotation({ -90.0f, 0.0f, 0.0f });

	//카메라 설정
	SpringArm = new FPSpringArmComponent(this);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 50.0f;
	SpringArm->SetRelativeLocation({ 0.0f, 1.2f, 0.0f });
	SpringArm->SetRelativeRotation({ 30.0f, 0.0f, 0.0f });
	SpringArm->bUsePawnControlRotation = true;

	PlayerCamera = new FPCameraComponent(this);
	PlayerCamera->SetupAttachment(SpringArm);


	FPAController* Controller = GetWorld()->GetController(0);

	if (Controller == nullptr)
		return;

	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Player::Move);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &Player::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWing", this, EKeyState::Pressed, &Player::SetScaleWing);
	Controller->GetInputComponent().BindMethod("IA_AttachHead", this, EKeyState::Down, &Player::AttachHead);
	Controller->GetInputComponent().BindMethod("IA_AttachShield", this, EKeyState::Down, &Player::AttachShield);
	Controller->GetInputComponent().BindMethod("IA_OnTorchLight", this, EKeyState::Down, &Player::OnTorch);
	Controller->GetInputComponent().BindMethod("IA_ZoomCamera", this, EKeyState::Pressed, &Player::CameraZoom);
	Controller->Possess(this);

}

void Player::BeginPlay()
{
	OneWindmillWing = static_cast<WindmillWing*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "WindmillWing"));
	TripleWing = static_cast<TripleWindmillWing*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "TripleWindmillWing"));
}

void Player::Tick()
{
	float mov = 180.0f * GetWorld()->GetGameTimer()->DeltaTime();
	//RootComponent->AddLocalRotation(FPVector3{ 0.0f, -mov, 0.0f });

	float RotateSpeed = 360.0f;
	ShieldPivot->AddLocalRotation(FPVector3{ 0.0f, 0.0f, RotateSpeed * GetWorld()->GetGameTimer()->DeltaTime() });


	__super::Tick();
}

void Player::Move(FInputValue Value)
{
	//std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldOffset(FPVector3{ move_x, 0.0f, move_y });
}

void Player::CameraMove(FInputValue value)
{
	FPAController* Controller = GetController();

	std::cout << "PlayerController Rotation [ " << Controller->GetActorRotation().x << " : " << Controller->GetActorRotation().y << " : " << Controller->GetActorRotation().z << " ]\n";
	std::cout << "SprtingArm Rotation [ " << SpringArm->GetComponentRotation().x << " : " << SpringArm->GetComponentRotation().y << " : " << SpringArm->GetComponentRotation().z << " ]\n";
	std::cout << "Actor Rotation [ " << GetActorRotation().x << " : " << GetActorRotation().y << " : " << GetActorRotation().z << " ]\n";
	std::cout << "Input [ " << value.X << " : " << value.Y << " ]\n";

	float mov = 30.0f;
	float move_x = value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());

	AddControllerYawInput(-move_x);
	AddControllerPitchInput(move_y);
}

void Player::CameraZoom(FInputValue Value)
{
	std::cout << "CameraZoom : " << Value.X << " : " << Value.Y << "\n";
	float mov = 30.0f;
	float mov_size = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	SpringArm->TargetArmLength = SpringArm->TargetArmLength + mov_size;
}

void Player::SetScaleWing(FInputValue Value)
{
	if (OneWindmillWing != nullptr)
	{
		OneWindmillWing->SetScaleWing(Value);
	}

	if (TripleWing != nullptr)
	{
		TripleWing->SetScaleWing(Value);
	}
}

void Player::SetOneWindmillWing(WindmillWing* Wing)
{
	OneWindmillWing = Wing;
}

void Player::SetTripleWindmillWing(TripleWindmillWing* Wing)
{
	TripleWing = Wing;
}

void Player::AttachHead(FInputValue Value)
{

	if (OneWindmillWing != nullptr)
	{
		OneWindmillWing->AttachHead(Value);
	}
}

void Player::AttachShield(FInputValue Value)
{

	if (OneWindmillWing != nullptr)
	{
		OneWindmillWing->AttachShield(Value);
	}
}

void Player::OnTorch(FInputValue Value)
{
	//TorchLight가 없다면
	if(TorchLight == nullptr)
	{ 
		//Torch Light 생성, 손에 붙임
		TorchLight =static_cast<ATorchLight*>(this->GetWorld()->SpawnActor("ATorchLight", "PlayerTorch"));
		TorchLight->AttachToComponent(Hand, "TorchPoint");
	}
	TorchLight->OnTorch();
}

FPSceneComponent* Player::GetShieldPivot()
{
	return ShieldPivot;
}

