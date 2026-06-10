#pragma once

#include "mcx9types.h"

/////////////////////////////////////////////////////////////
//
// 인터페이스/클래스 전방 선언
//
mcinterface IMia;
mcinterface IMiaDevice9;

typedef IMiaDevice9  MIADEVICE9;
typedef IMiaDevice9* LPMIADEVICE9;
typedef IMiaDevice9* LPDEVICE;

#define MIA_VERSION 9


/////////////////////////////////////////////////////////////
//
// 렌더링 자원 기반 인터페이스
// : IDirect3DResource9 대응
//
mcinterface IMiaResource9 : public mcIUnknown
{
	//자원 버퍼 획득
	virtual void* GetBuffer(void) pure;
	virtual int		GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;
};


/////////////////////////////////////////////////////////////
//
// 정점 버퍼 운용 인터페이스
// : IDirect3DVertexBuffer9 대응
//
mcinterface IMiaVertexBuffer9 : public IMiaResource9
{

	virtual int		Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) pure;
	virtual int		Unlock(void) pure;
	virtual int		GetDesc(_out_ B3MVERTEXBUFFER_DESC* pDesc) pure;

	//자원 버퍼 획득
	virtual void* GetBuffer(void) pure;
	virtual UINT	GetVertexCount(void) pure;
	virtual int		GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;

};

typedef IMiaVertexBuffer9* LPMIAVERTEXBUFFER9;
typedef IMiaVertexBuffer9* LPVBUFFER;
typedef IMiaVertexBuffer9* LPVB;


/////////////////////////////////////////////////////////////
//
// 각종 렌더링 '디바이스 Device' 들을 관리할 최상위 인터페이스
// : Direct3D 대응
//
mcinterface IMia : public mcIUnknown
{
	virtual int CreateDevice(HWND hwnd, MIAPRESENT_PARAMETERS* pp, DWORD vp, LPMIADEVICE9* pDev) pure;
};

typedef IMiaDevice9 MIADEVICE9;
typedef IMia* LPMIA;

////////////////////////////////
// Mia 개체 생성 함수 (D3D9 대응)
IMia* MiaCreate9(DWORD ver);



////////////////////////////////
// 인터페이스 선언
mcinterface IMiaDevice9 : public mcIUnknown
{
	////////////////////////////////
	// 스왑체인 및 렌더타겟 운용 메소드
public:
	virtual int  BeginScene() pure;
	virtual int  EndScene() pure;
	virtual int  Clear(COLORREF color) pure;
	virtual int  Present() pure;

	////////////////////////////////
	// 기하 버퍼 및 렌더링 메소드들
public:
	virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3MPOOL Pool, _out_ IMiaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle) pure;
	virtual int SetStreamSource(UINT StreamNumber, IMiaVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) pure;
	virtual int SetFVF(DWORD FVF) pure;
	virtual int DrawPrimitive(B3MPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) pure;
	
	////////////////////////////////
	// 렌더링 상태 조절 메소드들
	virtual int SetRenderState(B3MRENDERSTATETYPE State, DWORD Value) pure;
	virtual int GetRenderState(B3MRENDERSTATETYPE State, DWORD* pValue) pure;

	////////////////////////////////
	// 멤버데이터 접근자 Accessors
public:
	virtual HDC	 GetRT() pure;
	virtual COLORREF  GetBkColor() pure;
};

typedef IMiaDevice9* LPMIADEVICE9;
