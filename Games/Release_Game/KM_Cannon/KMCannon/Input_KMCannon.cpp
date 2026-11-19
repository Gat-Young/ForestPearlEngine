#include "Input_KMCannon.h"
#include "API_KMCannon.h"
#include "ForestPearlEngine/Object/Components/InputComponent.h"

const char* Input_KMCannon::IA_NAME = "IA_KMCannon";

namespace
{
	const USHORT KEY_FIRE = VK_RETURN;
	const USHORT KEY_ALL = '1';
	const USHORT KEY_LEFT = '2';
	const USHORT KEY_RIGHT = '3';

	bool IsDown(USHORT Key)
	{
		FPInputSystem* Input = static_cast<FPInputSystem*>(FPGameInstance::Get().GetInputSystem());
		return Input != nullptr && Input->bIsKeyDown(Key);
	}
}

void Input_KMCannon::AddMappings(FPInputComponent& Input)
{
	FModifyInfo Trigger = { ESwizzle::XYZ, ENegative::Positive };
	for (USHORT Key : { KEY_FIRE, KEY_ALL, KEY_LEFT, KEY_RIGHT })
		Input.AddMappingKey(IA_NAME, Key, Trigger);
}

//Enter 나 숫자가 눌린 순간 : 지금 눌려 있는 키 조합으로 판단
void Input_KMCannon::OnKeyDown(FPWorld* World)
{
	if (!IsDown(KEY_FIRE)) return;		//숫자만 누른 경우 : 아무것도 안 함

	if (IsDown(KEY_ALL))        API_KMCannon::FireBroadside(World, API_KMCannon::ESide::All);
	else if (IsDown(KEY_LEFT))  API_KMCannon::FireBroadside(World, API_KMCannon::ESide::Left);
	else if (IsDown(KEY_RIGHT)) API_KMCannon::FireBroadside(World, API_KMCannon::ESide::Right);
	//else                        API_KMCannon::FireAll(World);	//Enter 만
}