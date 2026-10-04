#pragma once

/////////////////////////////////////////////////////////////////////////////// 
//
// 시스템 / 플랫폼 헤더
//
#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "string"
#include "vector"
#include "algorithm"
using namespace std;

/////////////////////////////////////////////////////////////////////////////// 
//
// Dohwa SW Renderer 헤더

//
#include "DohwaMath.h"			//수학 라이브러리

// Dohwa SWR 장치/공통 인터페이스
#include "DohwaDefine.h"		//!< Dohwa SWR 기본/공통 상수 정의
#include "DOHWAUnknownbase.h"	//!< Dohwa SWR COM 헤더, 기반 인터페이스 헤더
#include "DOHWAx9types.h"		//!< Dohwa SWR 자료형 정의 : D3D9 대응
#include "DOHWAx9.h"			//!< Dohwa SWR 장치 헤더 : D3D9 대응



// Dohwa 확장 기능 클래스

#include "DohwaGraphics.h"		//Dohwa SWR 그래픽스 엔진