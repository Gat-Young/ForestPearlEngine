#include "InputMappingContext.h"
#include "InputAction.h"
#include "../../Define/FPMath.h"
#include <iostream>

void FPInputMappingContext::AddMappingKey(std::string IAName, USHORT VKey, FModifyInfo MappingInfo)
{
	//std::cout << "AddMappingKey :: " << VKey << "\n";
	MappingKeys.insert({ VKey, { IAName, MappingInfo } });
}

bool FPInputMappingContext::SearchMappingInfo(USHORT VKey, std::string& IAName, FModifyInfo& OutMappingInfo)
{
	if (MappingKeys.find(VKey) != MappingKeys.end())
	{
		IAName = MappingKeys[VKey].first;
		OutMappingInfo = MappingKeys[VKey].second;
		return true;
	}
	else
	{
		return false;
	}
}

bool FPInputMappingContext::GetMappedKeys(const std::string& IAName, std::set<USHORT>& MappedKeys)
{
	for (auto& [key, pairVal] : MappingKeys)
	{
		auto& [name, modInfo] = pairVal;
		if (name == IAName)
		{
			MappedKeys.insert(key);
		}
	}

	return !MappedKeys.empty();
}
