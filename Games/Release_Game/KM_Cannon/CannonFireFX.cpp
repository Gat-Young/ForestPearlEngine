#include "CannonFireFX.h"
#include "ForestPearlEngine/FPSceneComponent.h"
#include "ForestPearlEngine/FPStaticMeshComponent.h"
#include "ForestPearlEngine/FPPointLightComponent.h"
#include "ForestPearlEngine/FPMaterialInterface.h"
#include "ForestPearlEngine/FPAssetManager.h"
#include "ForestPearlEngine/FPGameInstance.h"
#include "ForestPearlEngine/FPGameTimer.h"
#include "ForestPearlEngine/FPWorld.h"
#include <cstdlib>
#include <iostream>

//보면서 조절할 값들 (Cena 값 그대로 시작)
namespace
{
	const float MUZZLE_TIME  = 0.1f;		//총구 불꽃 보이는 시간 (초)
	const float FRONT_SIZE   = 1.5f;		//정면 별 크기 (m)
	const float SIDE_LEN     = 3.0f;		//옆 불꽃 길이 : 640×480 = 4 : 3
	const float SIDE_WIDTH   = 2.25f;
	const float SIDE_BACK    = 0.3f;		//옆 불꽃 왼쪽 꽃잎을 총구 뒤로 이만큼

	const float FLASH_TIME   = 0.3f;		//섬광 빛 시간
	const float FLASH_POWER  = 5.0f;		//1 넘게 = 순간 과다 노출
	const FPVector3 FLASH_COL(1.0f, 0.6f, 0.2f);

	const int   SMOKE_BURST  = 12;			//한 번 쏠 때 연기 수
	const float SMOKE_ALPHA  = 0.6f;		//연기 진하기

	const int   PRIO_SMOKE   = 100;			//불투명 물체(0) 다 그린 뒤
	const int   PRIO_FLASH   = 110;			//연기 위에 더하기

	float Rand(float a, float b) { return a + (b - a) * (rand() / (float)RAND_MAX); }
}

//판 1장 : Plane 메쉬 + 이펙트 머티리얼 (판마다 새 머티리얼 = 색 · 크기를 따로 바꿀 수 있게)
FPStaticMeshComponent* CannonFireFX::MakeQuad(const char* MaterialName, int Priority)
{
	FPStaticMeshComponent* Mesh = new FPStaticMeshComponent(this, "Plane_StaticMesh");
	Mesh->SetupAttachment(Root);

	FPAssetManager* AssetManager = static_cast<FPAssetManager*>(FPGameInstance::Get().GetAssetManager());
	FPMaterialInterface* Material = AssetManager->MakeMaterial(MaterialName);
	if (Material != nullptr) Mesh->SetMaterial(Material);
	else std::cout << "[CannonFireFX] MakeMaterial failed : " << MaterialName << "\n";

	Mesh->SetPriority(Priority);
	Mesh->SetActive(false);
	return Mesh;
}

void CannonFireFX::Initialize()
{
	Root = new FPSceneComponent(this);
	SetRootComponent(Root);

	for (Puff& p : Smoke) p.Mesh = MakeQuad("FX_Smoke_Material", PRIO_SMOKE);
	FlashFront = MakeQuad("FX_MuzzleFront_Material", PRIO_FLASH);
	FlashSide  = MakeQuad("FX_MuzzleSide_Material", PRIO_FLASH);

	//섬광 빛 : 평소 꺼짐 (횃불과 같은 감쇠 1 / (0.1d + 0.1d²))
	FlashLight = new FPPointLightComponent(this);
	FlashLight->SetupAttachment(Root);
	FPVector4 Attenuation = { 0.0f, 0.1f, 0.1f, 1.0f };
	FlashLight->SetAttenuation(Attenuation);
	FlashLight->SetRange(100.0f);
	FlashLight->SetActive(false);
}

void CannonFireFX::BeginPlay()
{
	//레벨 JSON 의 위치와 상관없이 월드 원점에 둠 → 자식 상대 위치 = 월드 위치
	SetActorLocation({ 0.0f, 0.0f, 0.0f });
	SetActorRotation({ 0.0f, 0.0f, 0.0f });
	SetActorScale3D({ 1.0f, 1.0f, 1.0f });
}

void CannonFireFX::Fire(const FPVector3& Pos, const FPVector3& Dir)
{
	//섬광 : 자리 · 방향 기억
	MuzzleT = MUZZLE_TIME;
	LightT = FLASH_TIME;
	MuzzlePos = Pos;
	MuzzleDir = Dir;
	MuzzleRot = Rand(0, 2 * PI);
	FlashLight->SetRelativeLocation(Pos);
	FlashLight->SetActive(true);

	//연기 : 포신 방향으로 확 뿜어졌다가 공기 저항으로 멈추고 위로 퍼짐
	for (int i = 0; i < SMOKE_BURST; i++)
	{
		Puff* p = EmitSmoke(Pos + Dir * Rand(0.0f, 0.5f));
		if (p == nullptr) return;
		p->Vel = Dir * Rand(1.5f, 5.0f) + FPVector3(Rand(-0.4f, 0.4f), Rand(0.2f, 0.8f), Rand(-0.4f, 0.4f));
		p->Life = Rand(2.0f, 3.5f);
		p->Size = Rand(0.5f, 0.9f);
	}
}

//빈 칸 하나에 새 연기 (빈 칸 없으면 nullptr) : 부르는 쪽에서 속도 · 수명을 덮어씀
CannonFireFX::Puff* CannonFireFX::EmitSmoke(const FPVector3& Pos)
{
	for (Puff& p : Smoke)
	{
		if (p.bLive) continue;
		p.Pos = Pos;
		p.Vel = FPVector3(Rand(-0.15f, 0.15f), Rand(0.8f, 1.2f), Rand(-0.15f, 0.15f));
		p.Age = 0;
		p.Life = Rand(3.5f, 5.5f);
		p.Size = Rand(0.4f, 0.6f);
		p.Rot = Rand(0, 2 * PI);			//저마다 다르게 돌려서 같은 그림 티 안 나게
		p.Spin = Rand(-0.5f, 0.5f);
		p.bLive = true;
		p.Mesh->SetActive(true);
		return &p;
	}
	return nullptr;
}

// ponytail: 연기끼리 먼 것부터 정렬 안 함 (우선순위 고정) → 겹친 퍼프 순서가 가끔 뒤바뀌어 보임.
//           티 나면 카메라 거리로 SetPriority 를 매 프레임 다시 주면 됨
void CannonFireFX::UpdateSmoke(float dTime)
{
	float drag = (std::max)(1 - 1.5f * dTime, 0.0f);		//v ← v × (1 − 1.5·dt) : 1초에 약 e⁻¹·⁵ ≈ 22% 로

	for (Puff& p : Smoke)
	{
		if (!p.bLive) continue;
		p.Age += dTime;
		if (p.Age > p.Life) { p.bLive = false;  p.Mesh->SetActive(false);  continue; }	//수명 끝 → 빈 칸

		p.Pos = p.Pos + p.Vel * dTime;
		p.Vel = p.Vel * drag;
		p.Vel.x += 0.3f * dTime;		//바람 : +X 로 조금씩 밀림
		p.Vel.y += 0.3f * dTime;		//뜨거운 연기는 위로
		p.Rot += p.Spin * dTime;

		float t = p.Age / p.Life;							//0 → 1
		float size = p.Size * (1 + 2.5f * t);				//점점 퍼짐
		float a = (std::min)(t * 5, 1.0f) * (1 - t);		//빨리 나타나서 천천히 사라짐
		float k = (std::min)(t * 1.5f, 1.0f);				//주황(불씨) → 회색(연기)
		FPVector4 Color = { 1.0f + (0.6f - 1.0f) * k, 0.5f + (0.6f - 0.5f) * k, 0.1f + (0.6f - 0.1f) * k, a * SMOKE_ALPHA };
		FPVector4 Size = { size, size, p.Rot, 0.0f };

		p.Mesh->SetRelativeLocation(p.Pos);
		p.Mesh->GetMaterial().SetParameter(FPShaderType::VS, "FXVertexCB", "Size", Size);
		p.Mesh->GetMaterial().SetParameter(FPShaderType::PS, "FXPixelCB", "Color", Color);
	}
}

void CannonFireFX::UpdateMuzzle(float dTime)
{
	//빛 : 남은 시간 비율만큼 세기 (power → 0), 다 되면 끔
	if (LightT > 0)
	{
		LightT -= dTime;
		float k = (std::max)(LightT, 0.0f) / FLASH_TIME * FLASH_POWER;
		FPVector4 LightColor = { FLASH_COL.x * k, FLASH_COL.y * k, FLASH_COL.z * k, 1.0f };
		FlashLight->SetLightColor(LightColor);
		if (LightT <= 0) FlashLight->SetActive(false);
	}

	MuzzleT = (std::max)(MuzzleT - dTime, 0.0f);
	bool bOn = MuzzleT > 0;
	FlashFront->SetActive(bOn);
	FlashSide->SetActive(bOn);
	if (!bOn) return;

	float k = MuzzleT / MUZZLE_TIME;			//1 → 0 : 확 켜졌다 꺼짐 (더하기라 색 = 세기)
	FPVector4 Color = { k, k, k, 1.0f };

	//정면 별 : 보통 빌보드, 빛나며 조금 커짐
	float front = FRONT_SIZE * (1.5f - 0.5f * k);
	FPVector4 FrontSize = { front, front, MuzzleRot, 0.0f };
	FlashFront->SetRelativeLocation(MuzzlePos);
	FlashFront->GetMaterial().SetParameter(FPShaderType::VS, "FXVertexCB", "Size", FrontSize);
	FlashFront->GetMaterial().SetParameter(FPShaderType::PS, "FXPixelCB", "Color", Color);

	//옆 불꽃 : 늘인 빌보드, u 축 = 진행 방향. 판 가운데 = (총구 − 조금 뒤) + 반 길이
	FPVector3 Center = MuzzlePos + MuzzleDir * (SIDE_LEN * 0.5f - SIDE_BACK);
	FPVector4 Axis = { MuzzleDir.x, MuzzleDir.y, MuzzleDir.z, 1.0f };
	FPVector4 SideSize = { SIDE_LEN, SIDE_WIDTH, 0.0f, 0.0f };
	FlashSide->SetRelativeLocation(Center);
	FlashSide->GetMaterial().SetParameter(FPShaderType::VS, "FXVertexCB", "Axis", Axis);
	FlashSide->GetMaterial().SetParameter(FPShaderType::VS, "FXVertexCB", "Size", SideSize);
	FlashSide->GetMaterial().SetParameter(FPShaderType::PS, "FXPixelCB", "Color", Color);
}

void CannonFireFX::Tick()
{
	float dTime = GetWorld()->GetGameTimer()->DeltaTime();
	UpdateSmoke(dTime);
	UpdateMuzzle(dTime);

	__super::Tick();		//위치를 바꾼 뒤 : 루트 → 자식 행렬 계산
}
