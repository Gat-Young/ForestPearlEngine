#pragma once
#include <map>
#include <string>
#include <set>
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
	void AddMappingKey(std::string IAName, USHORT VKey, FModifyInfo MappingInfo);
	bool SearchMappingInfo(USHORT VKey, std::string& IAName, FModifyInfo& OutMappingInfo);
	bool GetMappedKeys(const std::string& IAName, std::set<USHORT>& MappedKeys);
	//void RemoveMappingKey(USHORT VKey);

private:
	std::map<USHORT/*Key*/, std::pair<std::string, FModifyInfo>> MappingKeys;
};

