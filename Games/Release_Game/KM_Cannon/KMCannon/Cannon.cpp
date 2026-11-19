#include "Cannon.h"
#include "CannonFireFX.h"
#include "CannonBall.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"

#include "ForestPearlEngine/Utility/FPGameplayStatics.h"

//★ 총구 = 레벨에 월드 좌표로 놓은 CannonSocket 액터 "<대포이름>_Muzzle"
//  BeginPlay 에서 대포 메쉬에 "월드 위치 그대로" 붙임 → 그 뒤로는 배 · 대포를 따라감
//  위치 · 회전은 Assets/Level/KM_Cannon.json 에서 맞춤 (다시 빌드 안 해도 됨, 소켓의 +Z 가 포구 밖을 보게)
static const char* MUZZLE_SUFFIX = "_Muzzle";
static const FPVector3 MUZZLE_FORWARD(0.0f, 1.0f, 0.0f);	//총구 소켓의 앞 = 로컬 +Y (Blender 로컬 +Z)

//Child 를 Parent 에 붙이되 지금 월드 위치 · 회전 · 크기 유지 (엔진 AttachToComponent 는 상대값 유지 = 부모 자리로 튐)
static void AttachKeepWorld(FPActor* Child, FPSceneComponent* Parent)
{
	FTransform World = Child->GetActorTransform();
	Child->AttachToComponent(Parent);
	Child->SetActorLocation(World.Location);	//SetWorld* → 부모 기준 상대값 다시 계산
	Child->SetActorRotation(World.Rotation);
	Child->SetActorScale3D(World.Scale);
}

void Cannon::Initialize()
{
	Mesh = new FPStaticMeshComponent(this, "Canon_1_StaticMesh");
	//Mesh->SetupAttachment(RootComponent); //루트에 자식
	SetRootComponent(Mesh); //내가 루트
}

void Cannon::BeginPlay()
{
	FireFX = static_cast<CannonFireFX*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "CannonFireFX"));
	Balls = static_cast<CannonBall*>(FPGameplayStatics::GetActorOfClass(GetWorld(), "CannonBall"));

	FPActor* ShipActor = FPGameplayStatics::GetActorOfClass(GetWorld(), "Ship");

	//배가 있으면 배에 붙임 (배 없는 프로젝트에서도 대포 · 총구 · 발사는 동작)
	if (ShipActor != nullptr)
	{
		AttachToActor(ShipActor);

		// 배를 기준으로 한 상대 위치·회전·크기
		Mesh->SetRelativeLocation({ 2.8f, 3.45f, -8.0f });
		Mesh->SetRelativeRotation({ 0.0f, 0.0f, 0.0f });
		Mesh->SetRelativeScale3D({ 0.33f, 0.33f, 0.33f });
	}

	//대포 자리가 정해진 뒤에 총구 소켓을 붙임 (레벨에 적은 월드 좌표 = 이 순간의 대포 기준)
	std::vector<FPActor*> Sockets;
	FPGameplayStatics::GetAllActorsOfClass(GetWorld(), "CannonSocket", Sockets);
	for (FPActor* Socket : Sockets)
	{
		if (Socket->GetName() == GetName() + MUZZLE_SUFFIX)
		{
			Muzzle = Socket;
			AttachKeepWorld(Muzzle, Mesh);
			break;
		}
	}
}

void Cannon::Fire()
{
	if (Muzzle == nullptr) return;

	//이펙트가 필요할 때 총구 오브젝트의 월드 위치 · 방향만 읽음 (배 · 대포가 움직여도 지난 Tick 의 월드 값이 따라와 있음)
	FTransform World = Muzzle->GetActorTransform();
	FPVector3 Pos = World.Location;
	FPVector3 Dir = Rotate(World.QuaternionRotation, MUZZLE_FORWARD).Normalize();
	if (FireFX != nullptr) FireFX->Fire(Pos, Dir);
	if (Balls != nullptr) Balls->Launch(Pos, Dir);
}
