#pragma once
#include <map>
#include "../../Systems/KeyStateEnum.h"

class FPInputAction;

enum class ESwizzle
{
	XYZ,
	YZX,
	ZXY
};

enum class ENegative : bool
{
	Negative = false,
	Positive = true
};

struct FMappingInfo
{
	ESwizzle Swizzle = ESwizzle::XYZ;
	ENegative bIsPositive = ENegative::Positive;
};

typedef unsigned short USHORT;

class FPInputMappingContext
{
public:
	void AddMappingKey(USHORT VKey, FMappingInfo MappingInfo);
	bool SearchMappingInfo(USHORT VKey, FMappingInfo& OutMappingInfo);
	//void RemoveMappingKey(USHORT VKey);

private:
	std::map<USHORT/*Key*/, FMappingInfo> MappingKeys;

};

