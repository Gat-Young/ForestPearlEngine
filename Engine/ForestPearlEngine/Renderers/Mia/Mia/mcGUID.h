#pragma once

#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "vector"
#include "map"
#include "algorithm"

/////////////////////////////////////////////////////////////
//
// GUID 정의 구조체 : Windows/GUID 대응
//
struct MCID
{
    unsigned long  Data1;     //16진수 8개
    unsigned short Data2;     //16진수 4개
    unsigned short Data3;     //16진수 4개
    unsigned char  Data4[8];  //16진수 4개(2byte) + 16진수 6개(4byte)
};

typedef MCID  MCIID;
typedef MCID MCCLSID;

////////////////////////////////
// Mia IID 정의 : 인터페이스 별 고유 ID 지정
EXTERN_C const MCIID IID_mcIUnknown;
EXTERN_C const MCIID IID_IMia;
EXTERN_C const MCIID IID_IMiaDevice9;
EXTERN_C const MCIID IID_IMiaVertexBuffer9;

////////////////////////////////
// IID 동일 여부 검사 함수
BOOL mcIsEqualIID(const MCIID& id1, const MCIID& id2);



