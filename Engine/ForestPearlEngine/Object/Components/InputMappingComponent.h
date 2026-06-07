#pragma once
#include <map>
#include "../../Systems/KeyStateEnum.h"

class FPInputAction;

struct FMappingInfo
{
	FPInputAction* InputAction = nullptr;
	unsigned char ModifyInfo = 0; //0b 0000 x y z negative
};

typedef unsigned short USHORT;

class FPInputMappingComponent
{
public:
	void OnKeyStateChanged(USHORT VKey, EKeyState KeyState);
	void AddMappingKey(USHORT VKey, FMappingInfo MappingInfo);
	void RemoveMappingKey(USHORT VKey);

private:
	std::map<USHORT/*Key*/, FMappingInfo> MappingKeys;

};

