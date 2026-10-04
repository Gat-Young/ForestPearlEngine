#pragma once

////////////////////////////////
// 시스템 / 플랫폼 헤더
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"
#include "vector"
#include "algorithm"
#include "string"

////////////////////////////////
// Mia SW Renderer 헤더
// Yena SWR 장치/공통 인터페이스
#include "Define/mcDefine.h"                       // 기본/공통 상수 정의
#include "Debug/mcError.h"                        // 에러처리 헤더
#include "Core/mcGUID.h"                        // Mia SWR COM / GUID 헤더
#include "Core/mcUnknownbase.h"           // COM 헤더, 기반 인터페이스 헤더
#include "RenderingDevice/mcx9types.h"                    // 자료형 정의 : D3D9 대응
#include "RenderingDevice/mcx9.h"                             // 장치 헤더 : D3D9 대응
#include "Define/mcMath.h"
