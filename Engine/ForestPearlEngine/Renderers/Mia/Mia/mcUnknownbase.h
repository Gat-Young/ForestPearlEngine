#pragma once

////////////////////////////////
// 시스템 / 플랫폼 헤더
#include "windows.h"


/////////////////////////////////////////////////////////////
//
// 소프트웨어 COM 상수 정의
//
////////////////////////////////
// Interface 정의 : VC/COM 은 "combaseapi.h" 에 정의됨
#ifndef interface
	#define __STRUCT__ struct
	#define interface __STRUCT__
#else
	#define mcinterface __interface
#endif

////////////////////////////////
// (DX11 대응) Interface/GUID 관련 재정의 : VC/COM 은 "rpcndr.h" 에 정의됨
#define MC_IID    const MCID &                                  // 인터페이스 ID, GUID/REFFID 대응
//#define MC_REFIID const IID &                               // 인터페이스 ID, GUID/REFFID 대응
//#define MC_DECLSPEC_UUID(x)     __declspec(uuid(x))         //인터페이스 ID 컴파일러 지정,  COM/DECLSPEC_UUID 대응
//#define MC_DECLSPEC_NOVTABLE    __declspec(novtable)        //인터페이스 접근(생성)제한 컴파일러 옵션 옵션 설정, COM/DECLSPEC_NOVTABLE 대응
//#define MC_MIDL_INTERFACE(x)    struct MC_DECLSPEC_UUID(x) MC_DECLSPEC_NOVTABLE
////#define MC_INTERFACE            struct MC_DECLSPEC_NOVTABLE
//#define MC_INTERFACE(x)         struct MC_DECLSPEC_UUID(x) MC_DECLSPEC_NOVTABLE

////////////////////////////////
// (DX9 대응) Interface/GUID 관련 재정의 : VC/COM 은 "combaseapi.h" 에 정의됨
#define MC_DECLARE_INTERFACE(iface)                interface MC_DECLSPEC_NOVTABLE iface
#define MC_DECLARE_INTERFACE_(iface, baseiface)    interface MC_DECLSPEC_NOVTABLE iface : public baseiface

////////////////////////////////
// (DX9/11) 인터페이스 정의 매크로
#define BEGIN_INTERFACE
#define END_INTERFACE

////////////////////////////////
// SW-COM 호출 규약, COM/STDMETHODCALLTYPE 대응 (__stcall : Win32 API 기본 호출규약)
#define MC_STDMETHODCALLTYPE    __stdcall
#define MC_STDMETHOD(method)    virtual int method

////////////////////////////////
// 메소드 순수 가상함수 표시용
#ifndef pure
	#define pure =0
#endif

/////////////////////////////////////////////////////////////
//
// mcUnKnown : Mia SWR 최상위 인터페이스
//
mcinterface mcIUnknown
{
	virtual int QueryInterface(MC_IID riid, _out_ void** ppvObject) pure;

	virtual ULONG AddRef(void) pure;
	virtual ULONG Release(void) pure;
};
