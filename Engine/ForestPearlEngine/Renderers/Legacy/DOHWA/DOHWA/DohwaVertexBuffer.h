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
// B3YenaVertexBuffer9
// 정점 버퍼 구현 클래스 : IDirect3DVertexBuffer9 대응  
// 
//  정점버퍼
//  정점버퍼는 일련의 기하(Geometry)를 구성하는 정점 데이터가 저장되는 버퍼입니다.  
//  이 데이터들은 정점 단위규격(Vertex Format 또는 Vertex Declaration) 으로 배열화 저장되며 읽기 전용으로 GPU 에 전달됩니다.  
//  필요시 동적 정점 버퍼 (Dynamic Vertex Buffer) 를 구성하는 것도 가능합니다.
//  
//  정점버퍼 렌더링 
//  렌더링 장치는 렌더링 파이프라인에 설정된 정점버퍼를 읽어오며, 렌더링 기하구성(Primitive Topology) 
//  주문에 따라 데이터를 처리합니다. (Vertex Streaming)  
//  렌더링 파이프라인은 하나 이상의 정점버퍼를 연결(Binding) 할 수 있으며 (DX9, max.16)  
//  정점 조립 스테이지(Vertex Assembly 또는 Input Assembly Stage) 에서 재구성되어 사용됩니다.
//  
//  <DX9> IDirect3DVertexBuffer9 으로 정점버퍼를 운용합니다.  
//  <Dohwa> IDohwaVertexBuffer9 으로 DX 인터페이스를 대응합니다.  
// 
//  장치(Device)는 파이프라인에 설정된 "현재" 정점 버퍼를 사용하므로 렌더링 명령
//			(Draw / DrawPrimitive) 전에 장치에 연결(Binding) 되어야 합니다.  
// 
// 
/////////////////////////////////////////////////////////////////////////////// 

class DohwaVertexBuffer9 : public IDohwaVertexBuffer9
{
	friend class DohwaDevice9;
	friend class DohwaGraphicsEngine9;


protected:
	//레퍼런스 참조 카운트 
	ULONG m_ref = 0;

protected:
	void* m_pVBuffer;				//!< 정점 버퍼 : 실제 데이터가 저장됨.
	DWORD m_SizeInByte;				//!< 정점 버퍼 크기 (바이트)
	DWORD m_FVF;					//!< 정점 버퍼 규격 조합 플래그.
	UINT  m_Stride;					//!< 정점 버퍼 안의 1마디(정점 구조 하나) 의 크기.
	UINT  m_VtxCnt;					//!< 정점 개수.★
	BOOL  m_bLocked;				//!< 버퍼 잠금 상태 (On/Off)


	DOHWAVERTEXBUFFER_DESC m_Desc;	//!< 정점 버퍼 정보 

protected:
	//(내부) 버퍼 운용 함수들 
	int		Create(DOHWAVERTEXBUFFER_DESC desc);
	int		Create(UINT Length, DWORD FVF, DOHWAPOOL Pool);		//구형 호환.

	void* GetVBuffer() { return m_pVBuffer; }
	DWORD	GetSizeInByte() { return m_SizeInByte; }
	DWORD	GetFVF() { return m_FVF; }
	DWORD	GetStride() { return m_Stride; }
	UINT	GetVtxCnt(); //★	

	//(내부) 정점 데이터 접근 메소드들
	DOHWAVECTOR2		GetPos2(int index);	//직접접근용.
	DOHWAVECTOR3		GetPos3(int index);
	DWORD				GetDiffuse(int index);
	//DOHWAXVECTOR2		GetPos2	();				//간접접근용.
	//DOHWAXVECTOR3		GetPos3	();
	//DOHWAXCOLOR		GetDiffuse(); 


public:
	DohwaVertexBuffer9(void);
	virtual ~DohwaVertexBuffer9(void);


	//-----------------------------------------------------------------
	// 인터페이스 재정의
	// DX 와 (거의)동일한 시그니쳐(Signature) 를 구현하는 것이 목표입니다. 
	//-----------------------------------------------------------------
	virtual int   Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
	virtual int   Unlock(void);
	virtual int   GetDesc(_out_ DOHWAVERTEXBUFFER_DESC* pDesc);

	//자원 버퍼 획득 : Yena 전용
	virtual void* GetBuffer(void);
	virtual UINT  GetVertexCount(void);	//★
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData);

	//--------------------------------
	// 참조 카운트 메소드 재정의 
	//--------------------------------
	virtual ULONG AddRef(void);
	virtual ULONG Release(void);
	virtual int   QueryInterface(DH_IID riid, _out_ void** ppvObject);


};