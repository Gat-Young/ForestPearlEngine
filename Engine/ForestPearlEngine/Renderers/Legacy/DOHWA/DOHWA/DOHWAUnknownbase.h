#pragma once

///////////////////////////////////////////
// 
// 시스템 / 플랫폼 헤더
//
#include <Windows.h>
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "vector"
#include "map"
#include "algorithm"
using namespace std;



#define DHinterface __interface

//가독성 향상
#ifndef _in_
#define _in_            //input
#define _in_out_        //input & output
#define _out_           //output
#define _out_opt_       //output, option
#define _opt_           //option
#define _in_opt_        //input, option
#endif

//메소드 순수 가상 함수 표시
#ifndef pure
#define pure =0
#endif

//////////////////////////////////////////
//
// Dohwa 소프트웨어 COM 상수 정의

//DX11 대응 interface/GUID 관련 재정의
#define DH_IID	const DHID&									//인터페이스 ID, GUID/REFFID 대응
#define DH_DECLSPEC_UUID(x)     __declspec(uuid(x))         //인터페이스 ID 컴파일러 지정,  COM/DECLSPEC_UUID 대응
#define DH_DECLSPEC_NOVTABLE    __declspec(novtable)        //인터페이스 접근(생성)제한 컴파일러 옵션 설정, COM/DECLSPEC_NOVTABLE 대응, novtable : 이 클래스는 직접 객체로 만들지 않는 순수 인터페이스용 클래스이므로, 생성자/소멸자에서 vtable 관련 초기화 코드를 만들지 말라는 뜻
#define DH_MIDL_INTERFACE(x)    struct DH_DECLSPEC_UUID(x) DH_DECLSPEC_NOVTABLE
#define DH_INTERFACE(x)         struct DH_DECLSPEC_UUID(x) DH_DECLSPEC_NOVTABLE

//DX9 대응 interface/GUID 관련 재정의
#define DH_DECLARE_INTERFACE(iface)                interface DH_DECLSPEC_NOVTABLE iface
#define DH_DECLARE_INTERFACE_(iface, baseiface)    interface DH_DECLSPEC_NOVTABLE iface : public baseiface

//SW-COM 호출 규약
#define DH_STDMETHODCALLTYPE    __stdcall                   
#define DH_STDMETHOD(method)    virtual int method

///////////////////////
//
// DOHWA GUID 구조체
struct DHID
{
    unsigned long  Data1;       //16진수 8개
    unsigned short Data2;       //16진수 4개
    unsigned short Data3;       //16진수 4개
    unsigned char  Data4[8];    //16진수 4개(2byte) + 16진수 6개(4byte)	
};

typedef DHID DHIID;
typedef DHID DHCLSID;

//DOHWA IID 정의 : 인터페이스별 고유 ID  지정
//Interface IID 범례 = { "YNSW"(8), 엔진버전(4), API(4), 버전(2), 인터페이스명(4), 버전 + 부가정보(10) };
EXTERN_C const DHIID IID_DHIUnknown;
EXTERN_C const DHIID IID_IDohwa;
EXTERN_C const DHIID IID_IDohwaDevice9;
EXTERN_C const DHIID IID_IDohwaVertexBuffer9;

//DOHWA Interface 구현 클래스 ID (CID)
EXTERN_C const DHCLSID CLSID_Dohwa;
EXTERN_C const DHCLSID CLSID_DohwaDevice9;
EXTERN_C const DHCLSID CLSID_DohwaVertexBuffer9;

//IID 동일 여부 검사 함수
BOOL DHIsEqualIID(const DHIID& id1, const DHIID& id2);

DHinterface DHIUnknown
{
	virtual int QueryInterface(DH_IID riid, _out_ void** ppvObject) pure;

    virtual ULONG AddRef(void) pure;
    virtual ULONG Release(void) pure;
};

template<class T>
T* DhGetClassObject(DHIUnknown* DhInterface)
{
    T* pObj = dynamic_cast<T*>(DhInterface);
    if(DOHWA_INVALIED(pObj))
    {
        //에러 처리
        return nullptr;
    }

    //참조 카운트 증가
    // ...
    return pObj;
}