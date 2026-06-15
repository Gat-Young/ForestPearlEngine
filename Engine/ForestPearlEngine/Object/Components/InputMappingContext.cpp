#include "InputMappingContext.h"
#include "InputAction.h"
#include "../../Define/FPMath.h"
#include <iostream>

void FPInputMappingContext::AddMappingKey(std::string IANAme, USHORT VKey, FModifyInfo MappingInfo)
{
	//std::cout << "AddMappingKey :: " << VKey << "\n";
	MappingKeys.insert({ VKey, { IANAme, MappingInfo } });
}

bool FPInputMappingContext::SearchMappingInfo(USHORT VKey, std::string& IANAme, FModifyInfo& OutMappingInfo)
{
	if (MappingKeys.find(VKey) != MappingKeys.end())
	{
		IANAme = MappingKeys[VKey].first;
		OutMappingInfo = MappingKeys[VKey].second;
		return true;
	}
	else
	{
		return false;
	}
}
