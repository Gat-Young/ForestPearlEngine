#include "InputAction.h"

void FPInputAction::BindFunc(FBindInfo InBindInfo)
{
	for (FBindInfo BindInfo : BindInfos)
	{
		if (InBindInfo.CallState == BindInfo.CallState && &InBindInfo.BindFuncPtr == &BindInfo.BindFuncPtr)
		{
			return;
		}
	}

	BindInfos.push_back(InBindInfo);
}
