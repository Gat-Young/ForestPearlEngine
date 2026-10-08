#include "WindmillWing.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPAController.h"
#include "ForestPearlEngine/FPWorld.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"
#include "ForestPearlEngine/InputValue.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/Utility/FPGameplayStatics.h"
#include "Player.h"

void WindmillWing::Initialize()
{
	Wing = new FPStaticMeshComponent(this, "Windmill_Wing_StaticMesh");
	SetRootComponent(Wing);

	FPAController* Controller = GetWorld()->GetController(0);
	Controller->GetInputComponent().BindMethod("IA_SetScaleWing", this, EKeyState::Pressed, &WindmillWing::SetScaleWing);
}

void WindmillWing::BeginPlay()
{
	Body = FPGameplayStatics::GetActorOfClass(GetWorld(), "Windmill");
	Player = FPGameplayStatics::GetActorOfClass(GetWorld(), "Player");

	ShieldPivot = static_cast<class Player*>(Player)->GetShieldPivot();

	AttachToActor(Body, "WingPoint1");
}

void WindmillWing::Tick()
{
	FPGameTimer* GameTimer = static_cast<FPGameTimer*>(FPGameInstance::Get().GetGameTimer());

	float RotateSpeed = 180.0f;
	if (isHead) RotateSpeed *= 2;
	Wing->AddLocalRotation(FPVector3{ 0.0f,  RotateSpeed * GameTimer->DeltaTime(),0.0f });
	__super::Tick();
}


void WindmillWing::AttachHead(FInputValue Value)
{
	ScaleOffset = 1.0f;
	isHead = !isHead;

	std::cout << "Attach Head : ";
	if (isHead)
	{
		std::cout << "Player\n";
		isShield = false;
		AttachToActor(Player, "HeadPivot");
		Wing->SetRelativeScale3D({ 1.0f, 1.0f, 1.0f });
		Wing->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
		Wing->SetRelativeLocation({0.0f, 0.0f, 0.0f});
	}
	else
	{
		std::cout << "Windmill\n";
		AttachToActor(Body, "WingPoint1");
		Wing->SetRelativeScale3D({ 1.0f, 1.0f, 1.0f });
		Wing->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
		Wing->SetRelativeLocation({ 0.0f, 0.0f, 0.0f });
	}
}

void WindmillWing::AttachShield(FInputValue Value)
{
	ScaleOffset = 1.0f;
	isShield = !isShield;

	std::cout << "Attach Shield : ";
	if (isShield)
	{
		std::cout << "Player\n";
		isHead = false;
		AttachToComponent(ShieldPivot);
		Wing->SetRelativeScale3D({ 1.0f, 1.0f, 1.0f });
		Wing->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
		Wing->SetRelativeLocation({ 0.0f, 3.0f, 0.0f });
	}
	else
	{
		std::cout << "Windmill\n";
		AttachToActor(Body, "WingPoint1");
		Wing->SetRelativeScale3D({ 1.0f, 1.0f, 1.0f });
		Wing->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
		Wing->SetRelativeLocation({ 0.0f, 0.0f, 0.0f });
	}
}

void WindmillWing::SetScaleWing(FInputValue Value)
{
	FPPawn* ParentPawn = static_cast<FPPawn*>(RootComponent->GetAttachmentRoot()->GetOwner());
	FPAController* PawnController = ParentPawn->GetController();
	//자신이 붙어있는 오브젝트가 Possess가 아니라면 넘어감
	if ((PawnController != nullptr) && (ParentPawn != ParentPawn->GetController()->GetPawn())) return;

	float mov = 1.0f;
	float move_x = Value.X * mov * (GetWorld()->GetGameTimer()->DeltaTime());
	
	if (isShield)
	{
		ScaleOffset += move_x;

		bool isBig = ScaleOffset > 2.0f;
		bool isSmall = ScaleOffset < 0.3f;
		if (isBig) { ScaleOffset = 2.0f; return; }
		if (isSmall) { ScaleOffset = 0.3f; return; }

		Wing->AddLocalOffset({ 0.0f, move_x * 5, 0.0});

		std::cout << Wing->GetRelativeLocation().x << " : " << Wing->GetRelativeLocation().y << " : " << Wing->GetRelativeLocation().z << "\n";
	}

	Wing->SetWorldScale3D(Wing->GetComponentScale() + FPVector3{ move_x, move_x, move_x });
}