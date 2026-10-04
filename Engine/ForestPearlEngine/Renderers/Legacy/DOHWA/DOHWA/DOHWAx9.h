#pragma once

//////////////////////////////////////////
//
// DOHWA 인터페이스/클래스 전방 선언
//
DHinterface IDohwa;					//IDirect3D9 대응
DHinterface IDohwaDevice9;			//IDohwa3DDevice9 대응
DHinterface IDohwaVertexBuffer9;			//IDohwa3DDevice9 대응

#define DOHWA_VERSION 9;

////////////////////////////////////////
//
//DOHWA 자료형 선언
//
#include "DOHWAx9Types.h"

/////////////////////////////////////////////////////////////////////////////// 
//
// interface IDohwaResource9 : 렌더링 자원 운용 인터페이스
// 			 				 : IDirect3DResource9 대응 ★ 
// 
DHinterface IDohwaResource9 : public DHIUnknown
{
	//자원 버퍼 획득 : DOHWA 전용
	virtual void* GetBuffer(void) pure;
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;
};

/////////////////////////////////////////////////////////////////////////////// 
//
// interface IDohwaVertexBuffer9 : 정점 버퍼 운용 인터페이스
//								: IDirect3DVertexBuffer9 대응 ★ 
// 

DHinterface IDohwaVertexBuffer9 : public IDohwaResource9
{
	virtual int Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) pure;
	virtual int Unlock(void) pure;
	virtual int GetDesc(_out_ DOHWAVERTEXBUFFER_DESC* pDesc) pure;

	//자원 버퍼 획득 : DOHWA 전용 
	virtual void* GetBuffer(void) pure;
	virtual UINT  GetVertexCount(void) pure;
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) pure;
};

///////////////////////////////////////
//
// DOHWA SWR 3D 운용 기반, 인터페이스 : IDirect3D9 대응
//
DHinterface IDohwa : public DHIUnknown
{
	//DOHWA 렌더링 디바이스 객체 생성 : D3D9 대응
	virtual int CreateDevice(
							HWND hwnd,						//[in] 디바이스의 렌더링 목표 윈도우 핸들.
							DOHWAPRESENT_PARAMETERS * pp,	//[in] 디바이스 화면 구성 정보.	
							DWORD vp,						//[in] 정점연산 방법 결정 (현재는 SW 만 가능)
							 _out_ IDohwaDevice9** pDev		//[out] 성공시 리턴받을 디바이스 개체 포인터.
							) pure;
};

/////////////////////////////////////////
//
// DOHWA 렌더링 디바이스 운용 인터페이스 : IDirect3DDevice 대응
//
DHinterface IDohwaDevice9 : public DHIUnknown
{
	public:
		// 스왑체인 및 렌더타겟 운용 메소드
		virtual int BeginScene() pure;
		virtual int EndScene() pure;
		virtual int Clear(COLORREF col) pure;
		virtual int Present() pure;

		//기하 버퍼 및 렌더링 메소드
		virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, DOHWAPOOL Pool, _out_ IDohwaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle) pure;
		virtual int SetStreamSource(UINT StreamNumber, IDohwaVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) pure;
		virtual int SetFVF(DWORD FVF) pure;
		virtual int DrawPrimitive(DOHWAPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) pure;

		//렌더링 상태 조정 메소드
		virtual int SetRenderState(DOHWARENDERSTATETYPE State, DWORD Value) pure;
		virtual int GetRenderState(DOHWARENDERSTATETYPE State, DWORD* pValue) pure;


		// 변환 행렬 설정.
		virtual int SetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* mTM) pure;
		virtual int GetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* mTM) pure;

		//DOHWA  전용
		virtual HDC		 GetRT() pure;
		virtual COLORREF GetBkColor() pure;
};

//DOHWA 개체 생성 함수 (D3D9 대응)
IDohwa* DohwaCreate9(DWORD ver);