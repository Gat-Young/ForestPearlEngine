#include "InputMappingContext.h"
#include "InputAction.h"
#include "../../Define/FPMath.h"
#include <iostream>

void FPInputMappingContext::AddMappingKey(USHORT VKey, FMappingInfo MappingInfo)
{
	//std::cout << "AddMappingKey :: " << VKey << "\n";
	MappingKeys.insert({ VKey, MappingInfo });
}

bool FPInputMappingContext::SearchMappingInfo(USHORT VKey, FMappingInfo& OutMappingInfo)
{
	if (MappingKeys.find(VKey) != MappingKeys.end())
	{
		OutMappingInfo = MappingKeys[VKey];
		return true;
	}
	else
	{
		return false;
	}
}
