#pragma once
#include <map>
#include <string>
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

struct FModifyInfo
{
	ESwizzle Swizzle = ESwizzle::XYZ;
	ENegative bIsPositive = ENegative::Positive;
};

typedef unsigned short USHORT;

class FPInputMappingContext
{
public:
	void AddMappingKey(std::string IANAme, USHORT VKey, FModifyInfo MappingInfo);
	bool SearchMappingInfo(USHORT VKey, std::string& IANAme, FModifyInfo& OutMappingInfo);
	//void RemoveMappingKey(USHORT VKey);

private:
	std::map<USHORT/*Key*/, std::pair<std::string, FModifyInfo>> MappingKeys;
};

