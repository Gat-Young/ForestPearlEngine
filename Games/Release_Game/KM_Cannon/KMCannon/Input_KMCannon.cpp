#include "Input_KMCannon.h"
#include "API_KMCannon.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"

const char* Input_KMCannon::IA_NAME = "IA_KMCannon";

namespace
{
	using API_KMCannon::ESide;

	const USHORT KEY_FIRE = VK_RETURN;

	//Enter 누른 채 숫자 : 어느 뱃전을, 빛을 켜고 쏠지
	struct Shot { USHORT Key; ESide Side; bool bLight; };
	const Shot SHOTS[] =
	{
		{ '1', ESide::All,   true  },
		{ '2', ESide::Left,  true  },
		{ '3', ESide::Right, true  },
		{ '4', ESide::All,   false },
		{ '5', ESide::Left,  false },
		{ '6', ESide::Right, false },
	};

	bool IsDown(USHORT Key)
	{
		FPInputSystem* Input = static_cast<FPInputSystem*>(FPGameInstance::Get().GetInputSystem());
		return Input != nullptr && Input->bIsKeyDown(Key);
	}
}

void Input_KMCannon::AddMappings(FPInputComponent& Input)
{
	FModifyInfo Trigger = { ESwizzle::XYZ, ENegative::Positive };
	Input.AddMappingKey(IA_NAME, KEY_FIRE, Trigger);
	for (const Shot& S : SHOTS)
		Input.AddMappingKey(IA_NAME, S.Key, Trigger);
}

//Enter 나 숫자가 눌린 순간 : 지금 눌려 있는 키 조합으로 판단 (Enter 만, 숫자만 = 아무것도 안 함)
void Input_KMCannon::OnKeyDown(FPWorld* World)
{
	if (!IsDown(KEY_FIRE)) return;

	for (const Shot& S : SHOTS)
	{
		if (IsDown(S.Key))
		{
			API_KMCannon::FireBroadside(World, S.Side, S.bLight);
			return;
		}
	}
}
