#pragma once
#include <vector>
#include <functional>
#include "../../Define/FPMath.h"
#include "../../InputValue.h"

struct FBindInfo
{
	enum EKeyState CallState;
	void* BindObj;
	std::function<void(FInputValue)> BindFuncPtr;
};

class FPInputAction
{
public:
	std::vector<FBindInfo>& GetBindInfos() { return BindInfos; }
	void BindFunc(FBindInfo InBindInfo);

private:
	std::vector<FBindInfo> BindInfos;
};