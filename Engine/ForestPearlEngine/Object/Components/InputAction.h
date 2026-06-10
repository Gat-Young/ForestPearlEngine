#pragma once

#include "../../Systems/KeyStateEnum.h"
#include "../../Define/FPMath.h"
#include <functional>

class FPInputAction
{
public:
	void OnInputAction(EKeyState KeyState, FPVector2 KeyValue);

    template<typename TObj>
    void BindMethod(TObj* BindActor, EKeyState BindKeyState, void(TObj::* FuncPtr)(FPVector2))
    {
        BindFuncPtr = [BindActor, FuncPtr](FPVector2 val)
            {
                (BindActor->*FuncPtr)(val);
            };
        CallKeyState = BindKeyState;
    }

private:
	EKeyState CallKeyState;
	std::function<void(FPVector2)> BindFuncPtr = nullptr;
};