#include "Ship.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPCameraComponent.h"
#include "ForestPearlEngine/FPSpringArmComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include <iostream>

void Ship::Initialize()
{

	//JosenShip Model
	const char* JosenShipMeshNames[] =
	{
		"SM_Anchor_StaticMesh",
		"SM_Bollard_StaticMesh",
		"SM_Bollard1_StaticMesh",
		"SM_Bollard2_StaticMesh",
		"SM_Bollard3_StaticMesh",
		"SM_Cauldron01a1_StaticMesh",
		"SM_Cauldron01a2_StaticMesh",
		"SM_Cauldron01a3_StaticMesh",
		"SM_Cauldron01a4_StaticMesh",
		"SM_Cauldron02a1_StaticMesh",
		"SM_Cauldron02a2_StaticMesh",
		"SM_Cauldron02a3_StaticMesh",
		"SM_Cauldron02a4_StaticMesh",
		"SM_Cauldron03a1_StaticMesh",
		"SM_Cauldron03a2_StaticMesh",
		"SM_Cauldron03a3_StaticMesh",
		"SM_Cauldron03a4_StaticMesh",
		"SM_Door01a_StaticMesh",
		"SM_Door01a1_StaticMesh",
		"SM_Door01a2_StaticMesh",
		"SM_Door01a3_StaticMesh",
		"SM_Door01a4_StaticMesh",
		"SM_Door01a5_StaticMesh",
		"SM_Door01a6_StaticMesh",
		"SM_Door01a7_StaticMesh",
		"SM_Door01a8_StaticMesh",
		"SM_Door01a9_StaticMesh",
		"SM_Door01a10_StaticMesh",
		"SM_Door01a11_StaticMesh",
		"SM_Door01a12_StaticMesh",
		"SM_Door01a13_StaticMesh",
		"SM_Door01a14_StaticMesh",
		"SM_Door01a15_StaticMesh",
		"SM_Door01a16_StaticMesh",
		"SM_Door01a17_StaticMesh",
		"SM_Door01a18_StaticMesh",
		"SM_Door01a19_StaticMesh",
		"SM_Door01a20_StaticMesh",
		"SM_Door01a21_StaticMesh",
		"SM_Door01a22_StaticMesh",
		"SM_Door01a23_StaticMesh",
		"SM_Door01a24_StaticMesh",
		"SM_Door01a25_StaticMesh",
		"SM_Door01a26_StaticMesh",
		"SM_Door01a27_StaticMesh",
		"SM_Door01a28_StaticMesh",
		"SM_Door01a29_StaticMesh",
		"SM_Door01a30_StaticMesh",
		"SM_Door01a31_StaticMesh",
		"SM_Door01a32_StaticMesh",
		"SM_Door01a33_StaticMesh",
		"SM_Fence01a_StaticMesh",
		"SM_Fence01a1_StaticMesh",
		"SM_Fence01a2_StaticMesh",
		"SM_Fence01a3_StaticMesh",
		"SM_Fence01a4_StaticMesh",
		"SM_Fence01a5_StaticMesh",
		"SM_Fence01a6_StaticMesh",
		"SM_Fence01a7_StaticMesh",
		"SM_Fence01a8_StaticMesh",
		"SM_Fence01a9_StaticMesh",
		"SM_Fence01a10_StaticMesh",
		"SM_Fence01a11_StaticMesh",
		"SM_Fence01a17_StaticMesh",
		"SM_Fence01a18_StaticMesh",
		"SM_Fence01a19_StaticMesh",
		"SM_Fence01a20_StaticMesh",
		"SM_Fence01a21_StaticMesh",
		"SM_Fence01a22_StaticMesh",
		"SM_Fence01a23_StaticMesh",
		"SM_Fence01a24_StaticMesh",
		"SM_Fence01a25_StaticMesh",
		"SM_Fence01a26_StaticMesh",
		"SM_Fence01a27_StaticMesh",
		"SM_Fence01a28_StaticMesh",
		"SM_Fence01a29_StaticMesh",
		"SM_Fence01a30_StaticMesh",
		"SM_Fence01a31_StaticMesh",
		"SM_Fence01a32_StaticMesh",
		"SM_Fence01b_StaticMesh",
		"SM_Fence01b1_StaticMesh",
		"SM_Fence01b2_StaticMesh",
		"SM_Fence01b3_StaticMesh",
		"SM_Fence01b4_StaticMesh",
		"SM_Fence01b5_StaticMesh",
		"SM_Fence01b6_StaticMesh",
		"SM_Fence01b7_StaticMesh",
		"SM_Fence01c_StaticMesh",
		"SM_Fence01c1_StaticMesh",
		"SM_Fence01c2_StaticMesh",
		"SM_Fence01c3_StaticMesh",
		"SM_Fence01c4_StaticMesh",
		"SM_Fence01c5_StaticMesh",
		"SM_Fence01c6_StaticMesh",
		"SM_Fence01c7_StaticMesh",
		"SM_Fence01c8_StaticMesh",
		"SM_Fence01c9_StaticMesh",
		"SM_Fence01c10_StaticMesh",
		"SM_Fence01c11_StaticMesh",
		"SM_Fence01c12_StaticMesh",
		"SM_Fence01c13_StaticMesh",
		"SM_Fence01c14_StaticMesh",
		"SM_Fence01c15_StaticMesh",
		"SM_Fence01c16_StaticMesh",
		"SM_Fence01c17_StaticMesh",
		"SM_Fence01c18_StaticMesh",
		"SM_Fence01c19_StaticMesh",
		"SM_Fence01c20_StaticMesh",
		"SM_Fence01c21_StaticMesh",
		"SM_Fence01c22_StaticMesh",
		"SM_Fence01c23_StaticMesh",
		"SM_Fence01c24_StaticMesh",
		"SM_Fence01c25_StaticMesh",
		"SM_Fence01c26_StaticMesh",
		"SM_Fence01c27_StaticMesh",
		"SM_Fence01c28_StaticMesh",
		"SM_Fence01c29_StaticMesh",
		"SM_Fence01c30_StaticMesh",
		"SM_Fence01c31_StaticMesh",
		"SM_Fence01c32_StaticMesh",
		"SM_Fence01c33_StaticMesh",
		"SM_Fence01c34_StaticMesh",
		"SM_Fence01c35_StaticMesh",
		"SM_Fence01c36_StaticMesh",
		"SM_Fence01c37_StaticMesh",
		"SM_Fence01c38_StaticMesh",
		"SM_Fence01c39_StaticMesh",
		"SM_Fence01c40_StaticMesh",
		"SM_Fence01c41_StaticMesh",
		"SM_Fence01c42_StaticMesh",
		"SM_Fence01c43_StaticMesh",
		"SM_Fence01c44_StaticMesh",
		"SM_Fence01c45_StaticMesh",
		"SM_Fence01c46_StaticMesh",
		"SM_Fence01c47_StaticMesh",
		"SM_Fence01c48_StaticMesh",
		"SM_Fence01c49_StaticMesh",
		"SM_Fence01c50_StaticMesh",
		"SM_Fence01c51_StaticMesh",
		"SM_Fence01c52_StaticMesh",
		"SM_Fence01d_StaticMesh",
		"SM_Fence01d1_StaticMesh",
		"SM_Fence02a_StaticMesh",
		"SM_Fence02a1_StaticMesh",
		"SM_Fence02a2_StaticMesh",
		"SM_Fence02a3_StaticMesh",
		"SM_Firewood_StaticMesh",
		"SM_Firewood1_StaticMesh",
		"SM_Firewood2_StaticMesh",
		"SM_Flag01a_StaticMesh",
		"SM_Flag01b_StaticMesh",
		"SM_Flag01b1_StaticMesh",
		"SM_Flag01c_StaticMesh",
		"SM_Flag01c1_StaticMesh",
		"SM_Floor01a_StaticMesh",
		"SM_Floor01a1_StaticMesh",
		"SM_Floor01a2_StaticMesh",
		"SM_Floor01a3_StaticMesh",
		"SM_Floor01a4_StaticMesh",
		"SM_Floor01a5_StaticMesh",
		"SM_Floor01a6_StaticMesh",
		"SM_Floor01a7_StaticMesh",
		"SM_Floor01a8_StaticMesh",
		"SM_Floor01a9_StaticMesh",
		"SM_Floor01a10_StaticMesh",
		"SM_Floor02a_StaticMesh",
		"SM_Floor02a1_StaticMesh",
		"SM_Floor02a2_StaticMesh",
		"SM_Handle_StaticMesh",
		"SM_Horong_StaticMesh",
		"SM_Hull_StaticMesh",
		"SM_Lintel01a_StaticMesh",
		"SM_Lintel02a_StaticMesh",
		"SM_Mast01a_StaticMesh",
		"SM_Mast02a_StaticMesh",
		"SM_Oar_StaticMesh",
		"SM_Oar1_StaticMesh",
		"SM_Oar2_StaticMesh",
		"SM_Oar3_StaticMesh",
		"SM_Oar4_StaticMesh",
		"SM_Oar5_StaticMesh",
		"SM_Oar6_StaticMesh",
		"SM_Oar7_StaticMesh",
		"SM_Oar8_StaticMesh",
		"SM_Oar9_StaticMesh",
		"SM_Oar10_StaticMesh",
		"SM_Oar11_StaticMesh",
		"SM_Oar12_StaticMesh",
		"SM_Oar13_StaticMesh",
		"SM_Oar14_StaticMesh",
		"SM_Oar15_StaticMesh",
		"SM_Pillar_StaticMesh",
		"SM_Pillar1_StaticMesh",
		"SM_Pillar2_StaticMesh",
		"SM_Pillar3_StaticMesh",
		"SM_Pillar4_StaticMesh",
		"SM_Pillar5_StaticMesh",
		"SM_Pillar6_StaticMesh",
		"SM_Pillar7_StaticMesh",
		"SM_Pillar8_StaticMesh",
		"SM_Pillar9_StaticMesh",
		"SM_Pillar10_StaticMesh",
		"SM_Pillar11_StaticMesh",
		"SM_Pillar12_StaticMesh",
		"SM_Pillar13_StaticMesh",
		"SM_Pillar14_StaticMesh",
		"SM_Pillar15_StaticMesh",
		"SM_Pillar16_StaticMesh",
		"SM_Pillar17_StaticMesh",
		"SM_Pillar18_StaticMesh",
		"SM_Pillar19_StaticMesh",
		"SM_Pillar20_StaticMesh",
		"SM_Pillar21_StaticMesh",
		"SM_Pillar22_StaticMesh",
		"SM_Pillar23_StaticMesh",
		"SM_Pillar24_StaticMesh",
		"SM_Pillar25_StaticMesh",
		"SM_Pillar26_StaticMesh",
		"SM_Pillar27_StaticMesh",
		"SM_Pillar28_StaticMesh",
		"SM_Pillar29_StaticMesh",
		"SM_Pillar30_StaticMesh",
		"SM_Pillar31_StaticMesh",
		"SM_Pillar32_StaticMesh",
		"SM_Pillar33_StaticMesh",
		"SM_Pillar34_StaticMesh",
		"SM_Pillar35_StaticMesh",
		"SM_Pillar36_StaticMesh",
		"SM_Pillar37_StaticMesh",
		"SM_Pillar38_StaticMesh",
		"SM_Pillar39_StaticMesh",
		"SM_Pillar40_StaticMesh",
		"SM_Pillar41_StaticMesh",
		"SM_Pillar42_StaticMesh",
		"SM_Pillar43_StaticMesh",
		"SM_Pillar44_StaticMesh",
		"SM_Pillar45_StaticMesh",
		"SM_Pillar46_StaticMesh",
		"SM_Pillar47_StaticMesh",
		"SM_Pillar48_StaticMesh",
		"SM_Pillar49_StaticMesh",
		"SM_Roof_StaticMesh",
		"SM_Rope01a1_StaticMesh",
		"SM_Rope01a2_StaticMesh",
		"SM_Rope01a3_StaticMesh",
		"SM_Rope01a4_StaticMesh",
		"SM_Rope01a5_StaticMesh",
		"SM_Rope01a6_StaticMesh",
		"SM_Rope01a7_StaticMesh",
		"SM_Rope01a8_StaticMesh",
		"SM_Rope01a9_StaticMesh",
		"SM_Rope01a10_StaticMesh",
		"SM_Rope01a11_StaticMesh",
		"SM_Rope02a_StaticMesh",
		"SM_Rope02a1_StaticMesh",
		"SM_Rope02a2_StaticMesh",
		"SM_Rope02a3_StaticMesh",
		"SM_Rope02a4_StaticMesh",
		"SM_Rope02a5_StaticMesh",
		"SM_Rope02a6_StaticMesh",
		"SM_Rope02a7_StaticMesh",
		"SM_Rope02a8_StaticMesh",
		"SM_Rope02a9_StaticMesh",
		"SM_Rope02a10_StaticMesh",
		"SM_Rope02a11_StaticMesh",
		"SM_Rope02a12_StaticMesh",
		"SM_Rope02a13_StaticMesh",
		"SM_Rope02a14_StaticMesh",
		"SM_Rope02a15_StaticMesh",
		"SM_Stair01a_StaticMesh",
		"SM_Stair01a1_StaticMesh",
		"SM_Stair02a_StaticMesh",
		"SM_Straw_Bag_StaticMesh",
		"SM_Straw_Bag1_StaticMesh",
		"SM_Straw_Bag2_StaticMesh",
		"SM_Straw_Bag3_StaticMesh",
		"SM_Straw_Bag4_StaticMesh",
		"SM_Straw_Bag5_StaticMesh",
		"SM_Straw_Bag6_StaticMesh",
		"SM_Straw_Bag7_StaticMesh",
		"SM_Straw_Bag8_StaticMesh",
		"SM_Wall01a_StaticMesh",
		"SM_Wall01a1_StaticMesh",
		"SM_Wall01a2_StaticMesh",
		"SM_Wall01a3_StaticMesh",
		"SM_Wall01a4_StaticMesh",
		"SM_Wall01a5_StaticMesh",
		"SM_Wall01a6_StaticMesh",
		"SM_Wall01a7_StaticMesh",
		"SM_Wall01a8_StaticMesh",
		"SM_Wall01a9_StaticMesh",
		"SM_Wall01a10_StaticMesh",
		"SM_Wall01a11_StaticMesh",
		"SM_Wall01a12_StaticMesh",
		"SM_Wall01a13_StaticMesh",
		"SM_Wall01b_StaticMesh",
		"SM_Wall01b1_StaticMesh",
		"SM_Wall02a_StaticMesh",
		"SM_Wall02a1_StaticMesh",
		"SM_Wall02a2_StaticMesh",
		"SM_Wall02b_StaticMesh",
		"SM_Wall02b1_StaticMesh",
		"SM_Wall02b2_StaticMesh",
		"SM_Wall02b3_StaticMesh",
		"SM_Wall03a_StaticMesh",
		"SM_Wall03b_StaticMesh",
		"SM_Wall03b1_StaticMesh",
		"SM_Wall04a_StaticMesh",
		"SM_Wall04a1_StaticMesh",
		"SM_Wall04a2_StaticMesh",
		"SM_Wall04a3_StaticMesh",
		"SM_Wall04a4_StaticMesh",
		"SM_Wall04a5_StaticMesh",
		"SM_Wall04a6_StaticMesh",
		"SM_Wall04a7_StaticMesh",
		"SM_Wall04a8_StaticMesh",
		"SM_Wall04a9_StaticMesh",
		"SM_Wall04a10_StaticMesh",
		"SM_Wall04a11_StaticMesh",
		"SM_Wall04a12_StaticMesh",
		"SM_Wall04a13_StaticMesh",
		"SM_Wall04a14_StaticMesh",
		"SM_Wall04a15_StaticMesh",
		"SM_Wall04a16_StaticMesh",
		"SM_Wall04a17_StaticMesh",
		"SM_Wall04a18_StaticMesh",
		"SM_Wall04a19_StaticMesh",
		"SM_Wall04a20_StaticMesh",
		"SM_Wall04a21_StaticMesh",
		"SM_Wall04a22_StaticMesh",
		"SM_Wall04a23_StaticMesh",
		"SM_Wall04a24_StaticMesh",
		"SM_Wall04a25_StaticMesh",
		"SM_Wall04a26_StaticMesh",
		"SM_Window_StaticMesh",
		"SM_Window1_StaticMesh",
		"SM_Window2_StaticMesh",
		"SM_Window3_StaticMesh",
		"SM_Window4_StaticMesh",
		"SM_Window5_StaticMesh"
	};

	for (const char* MeshName : JosenShipMeshNames)
	{
		FPStaticMeshComponent* MeshComponent = new FPStaticMeshComponent(this, MeshName);
		MeshComponent->SetupAttachment(RootComponent);
		JosenShipMeshes.push_back(MeshComponent);
	}

	

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

	Controller->GetInputComponent().BindMethod("IA_SetMoveTriangel", this, EKeyState::Pressed, &Ship::Move);
	Controller->GetInputComponent().BindMethod("IA_SetMoveCamera", this, EKeyState::Pressed, &Ship::CameraMove);
	Controller->GetInputComponent().BindMethod("IA_ZoomCamera", this, EKeyState::Pressed, &Ship::CameraZoom);
	Controller->Possess(this);

}

void Ship::BeginPlay()
{

}

void Ship::Tick()
{
	float mov = 180.0f * GetWorld()->GetGameTimer()->DeltaTime();
	//RootComponent->AddLocalRotation(FPVector3{ 0.0f, -mov, 0.0f });

	float RotateSpeed = 360.0f;
	ShieldPivot->AddLocalRotation(FPVector3{ 0.0f, 0.0f, RotateSpeed * GetWorld()->GetGameTimer()->DeltaTime() });


	__super::Tick();
}

void Ship::Move(FInputValue Value)
{
	//std::cout << "Actor Move [ " << GetActorLocation().x << " : " << GetActorLocation().y << " : " << GetActorLocation().z << " ]\n";
	float mov = 10.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	float move_y = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	RootComponent->AddWorldOffset(FPVector3{ move_x, 0.0f, move_y });
}

void Ship::CameraMove(FInputValue value)
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

void Ship::CameraZoom(FInputValue Value)
{
	std::cout << "CameraZoom : " << Value.X << " : " << Value.Y << "\n";
	float mov = 30.0f;
	float mov_size = Value.Y * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	SpringArm->TargetArmLength = SpringArm->TargetArmLength + mov_size;
}
