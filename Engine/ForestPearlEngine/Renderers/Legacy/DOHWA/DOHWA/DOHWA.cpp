#include "Dohwa.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// Dohwa SWR 구현 클래스 선언 : 외부 노출 방지용.★
//
#include "DohwaClass.h"
#include "DohwaVertexBuffer.h"
#include "DohwaGraphics.h"

//////////////////////////////////////////////////////////////////////////
//
// Dohwa 3D 운용 기반, 구현 클래스, IDirect3D9 대응
//
class Dohwa9 : public IDohwa
{
	protected:
		//레퍼런스 참조 카운트
		ULONG m_ref = 0;

	public:
		Dohwa9(void);
		virtual ~Dohwa9(void);


		//! Dohwa SWR 렌더링 디바이스 개체 생성 : D3D9 대응
		virtual int CreateDevice(
									HWND hwnd,						//[in] 디바이스의 렌더링 목표 윈도우 핸들.
									DOHWAPRESENT_PARAMETERS* pp,	//[in] 디바이스 화면 구성 정보.	
									DWORD vp,						//[in] 정점연산 방법 결정 (현재는 SW 만 가능)
									_out_ IDohwaDevice9** pDev		//[out] 성공시 리턴받을 디바이스 개체 포인터.
								) override;


		//참조 카운트 메소드 재정의
		virtual ULONG AddRef(void) override;
		virtual ULONG Release(void) override;
		virtual int   QueryInterface(DH_IID riid, _out_ void** ppvObject) override;

};

/////////////////////////////////////////
//
// DOHWA 렌더링 장치 Device 구현 클래스  : IDirect3DDevice 대응
//
class DohwaDevice9 : public IDohwaDevice9
{
	friend class Dohwa9;

	protected:
		//레퍼런스 참조 카운트 ★
		ULONG m_ref = 0;


	protected:

		// 장치 운용 기본 정보.
		HWND					 m_hWnd;
		DWORD					 m_VertexProcessing;

		// 렌더타겟(Back-Buffer) 구성용 핸들.
		HDC		 m_hRT;
		COLORREF m_BkColor;

		//렌더링 그래픽스 엔진
		DohwaGraphicsEngine9* m_pGraphics;



	protected:
		int Create(HWND hwnd, DOHWAPRESENT_PARAMETERS* pp, DWORD vp);
		

	public:
		DohwaDevice9(void);
		virtual ~DohwaDevice9(void);

		// 스왑체인 및 렌더타겟 운용 메소드
		virtual int BeginScene() override;
		virtual int EndScene() override;
		virtual int Clear(COLORREF col) override;
		virtual int Present() override;

		//기하 버퍼 및 렌더링 메소드
		virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, DOHWAPOOL Pool, _out_ IDohwaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle);
		virtual int SetStreamSource(UINT StreamNumber, IDohwaVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride);
		virtual int SetFVF(DWORD FVF);
		virtual int DrawPrimitive(DOHWAPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);

		//렌더링 상태 조정 메소드
		virtual int SetRenderState(DOHWARENDERSTATETYPE	State, DWORD Value);
		virtual int GetRenderState(DOHWARENDERSTATETYPE State, DWORD* pValue);

		// 변환 행렬 설정
		virtual int SetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* mTM);
		virtual int GetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* mTM);

		//	Dohwa 전용
		virtual HDC		 GetRT();
		virtual COLORREF GetBkColor();

		//참조 카운트 메서드
		virtual ULONG AddRef(void) override;
		virtual ULONG Release(void) override;
		virtual int   QueryInterface(DH_IID riid, _out_ void** ppvObject) override;

};


/////////////////////////////////////////////////////////////
// 
// DohwaCreate9 : Dohwa 최상위 인터페이스 생성 함수
//
IDohwa* DohwaCreate9(DWORD ver)
{
	//COM객체 생성
	Dohwa9* Dohwa9Object = new Dohwa9;
	assert(Dohwa9Object != NULL);

	//인터페이스 생성 RTTI
	IDohwa* IDohwaObject = nullptr;
	if (DOHWA_FAILED(Dohwa9Object->QueryInterface(IID_IDohwa, (void**)&IDohwaObject)))
	{
		//...ERROR...
	}

	return IDohwaObject;
}


//////////////////////////////////////////////////////////////
//
// class Dohwa9 : 정의와 구현
//				: IDirect3D9 대응
//
//////////////////////////////////////////////////////////////

Dohwa9::Dohwa9()
{

}

Dohwa9::~Dohwa9()
{

}

//////////////////////////////////////////////////////////////
//
// DOHWA 렌더링 디바이스 객체 생성 : D3D9 대응
//
//

int Dohwa9::CreateDevice(HWND hwnd,
							 DOHWAPRESENT_PARAMETERS* pp,
							 DWORD vp,
						     IDohwaDevice9** ppDevice)
{
	//입력 정보 확인 : 오류별로 리턴값을 다르게 하는 것을 추천.
	if (hwnd == NULL) return DOHWA_FAIL;
	if (pp == NULL) return DOHWA_FAIL;
	if (ppDevice == NULL) return DOHWA_FAIL;


	//디바이스 COM 객체 생성.
	DohwaDevice9* DohwaDeviceObj = new DohwaDevice9;
	assert(DohwaDeviceObj != NULL);
	DohwaDeviceObj->Create(hwnd, pp, vp);

	//인터페이스 질의 및 획득.(GUID 및 Query) ★
	//GUID 와 Query 를 사용하는 것이 보다 DX/COM 표준 운용에 가깝습니다.
	//획득한 인터페이스는 사용후 Release 를 호출해야 합니다. (안전한 메모리 해제)
	IDohwaDevice9* IDohwaDeviceObj = nullptr;
	if (DOHWA_FAILED(DohwaDeviceObj->QueryInterface(IID_IDohwaDevice9, (void**)&IDohwaDeviceObj)))
	{
		//...ERROR...
	}


	//외부에 리턴..W
	*ppDevice = IDohwaDeviceObj;

	return DOHWA_OK;
}


////////////////////////////////////////////////////////////////////////////////
//
// Dohwa9 참조 관련 메소드
//

ULONG Dohwa9::AddRef(void)
{
	return ++m_ref;
};

ULONG Dohwa9::Release(void)
{
	if (--m_ref <= 0) delete this;

	return m_ref;
};

int Dohwa9::QueryInterface(DH_IID riid, _out_ void** ppvObject)
{
	if (DHIsEqualIID(riid, IID_IDohwa))
	{
		AddRef();
		*ppvObject = dynamic_cast<IDohwa*>(this);
	}

	return DOHWA_OK;
}


/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
// 
// class DohwaDevice9  : 정의 와 구현
//					   : IDirect3DDevice9대응
//
/////////////////////////////////////////////////////////////////////////////// 


///////////////////////////////////////////////////////////////////////////////
//
DohwaDevice9::DohwaDevice9(void)
{
	m_hWnd = NULL;
	m_VertexProcessing = 0;

	m_hRT = NULL;
	m_BkColor = RGB(0, 0, 255);

	m_pGraphics = nullptr;
}

DohwaDevice9::~DohwaDevice9(void)
{
	SafeDelete(m_pGraphics);
}

/////////////////////////////////////////////////////////////////////////////// 
//
//! 지정 렌더링 디바이스 개체 생성 메소드.
//
int DohwaDevice9::Create(HWND hwnd,
	DOHWAPRESENT_PARAMETERS* pp,
	DWORD vp
)
{
	//입력 정보 확인 : 오류별로 리턴값을 다르게 하는 것을 추천.
	if (DOHWA_INVALIED(hwnd)) return DOHWA_FAIL;
	if (DOHWA_INVALIED(pp))   return DOHWA_FAIL;


	//그래픽스 엔진 생성 (v1.5.3) ★
	DohwaGraphicsEngine9* pGraphics = new DohwaGraphicsEngine9;
	ASSERT(pGraphics);

	//디바이스/그래픽스 엔진 초기화.
	DohwaGraphicsEngine9::OUTPUT_MERGE om = {};
	om.hWnd = hwnd;
	om.PresentParam = *pp;
	pGraphics->m_om = om;
	//pGraphics->m_om.VertexProcessing = vp;
	pGraphics->RenderTargetCreate();			//렌더타겟(백버퍼) 생성 <Dohwa> 1개만 지원.


	//디바이스 - 그래픽스 엔진 바인딩.
	pGraphics->SetDev(this);

	//디바이스 - 그래픽스 엔진 정보 참조.
	m_hWnd = hwnd;
	m_VertexProcessing = vp;
	m_pGraphics = pGraphics;
	m_hRT = pGraphics->GetDCRT();
	m_BkColor = DOHWAXCOLOR(0.25f, 0.25f, 0.25f, 1);

	return DOHWA_OK;
}


////////////////////////////////////////////////////////////////////////////////
//
// 렌더타겟 DC 핸들 획득
// 
// return	렌더타겟 DC 핸들
//
HDC DohwaDevice9::GetRT()
{
	m_hRT = m_pGraphics->GetDCRT();
	return m_hRT;

}




///////////////////////////////////////////////////////////////////////////////
//
// 렌더타겟 배경색 획득
//
COLORREF DohwaDevice9::GetBkColor()
{
	return m_BkColor;
}




///////////////////////////////////////////////////////////////////////////////
//
// 장면 그리기 시작 : 렌더링에 필요한 (디바이스의) 선위 작업을 수행.
//
int DohwaDevice9::BeginScene()
{
	m_pGraphics->BeginScene();

	return DOHWA_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
// 장면 그리기 종료 : 렌더링 종료에 필요한 (디바이스의) 후위 작업을 수행.
//
int DohwaDevice9::EndScene()
{
	m_pGraphics->EndScene();

	return DOHWA_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
// 렌더타겟 클리어.
//
// param  col	RT 을 지울 지정색.
// return		성공시 OK, 실패시 FALSE
//
int DohwaDevice9::Clear(COLORREF col)
{
	m_BkColor = col;
	m_pGraphics->Clear(col);

	return DOHWA_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
// RT 의 내용(렌더링된 장면)을 Front Buffer 에 출력합니다. "Flipping", "Swapping"
//
// param   없음
// return  오류시 YN_OK 이외의 값.
// note <Dohwa> 주요 렌더링 작업은 Dohwa GraphicsEngine 에서 처리합니다.
//
int DohwaDevice9::Present()
{
	return m_pGraphics->Present();
}




////////////////////////////////////////////////////////////////////////////////
//
//! 정점 버퍼 생성.
//! \note 생성된 정점 버퍼는 렌더링 전, 장치(Device)에 설정(Binding) 되어야 합니다.
//! 
//! \param	Length			정점 버퍼의 전체 크기 (바이트)	
//! \param	Usage			버퍼 용도 (미지정은 0)
//! \param	FVF				정점 규격
//! \param	Pool			버퍼-메모리 형식 : 시스템메모리 사용. (Yena 유일 옵션) 
//! \param	ppVertexBuffer	성공시 리턴되는 정점 버퍼 인터페이스 포인터
//! \param	pSharedHandle	공유 헨들 (기본 NULL)
//! \return		성공시 OK, 실패시 FALSE
//
int DohwaDevice9::CreateVertexBuffer(UINT Length,
	DWORD Usage,
	DWORD FVF,
	DOHWAPOOL Pool,
	_out_ IDohwaVertexBuffer9** ppVertexBuffer,
	_in_opt_ HANDLE* pSharedHandle
)
{

	//정점 버퍼 운용 개체 생성.
	DohwaVertexBuffer9* pB3VB = new DohwaVertexBuffer9;
	if (DOHWA_INVALIED(pB3VB)) return DOHWA_FAIL;

	//생성할 정점 버퍼 정보 기술
	//<Dohwa> 시스템 메모리를 사용하므로 필수 동작은 아니지만, 구조의 일관성을 위해 사용.
	//GPU 메모리(VRAM) 확보를 위해서 이 정도의 수고는 필요하다...정도로 이해해 봅시다.
	DOHWAVERTEXBUFFER_DESC desc = {};
	desc.Format = DOHWAFMT_VERTEXDATA;		//버퍼 포멧 : 정점 데이터용
	desc.Type = DOHWARTYPE_VERTEXBUFFER;	//버펴 형식 : 정점 버퍼
	desc.Usage = DOHWAUSAGE_WRITEONLY;		//버퍼 사용방식 : 읽기 전용, <주> 정점버퍼는 CPU "쓰기전용", GPU "읽기 전용" 임을 상기합시다.
	desc.Pool = Pool;						//버퍼 메모리 풀 형식
	desc.Size = Length;					//버퍼 크기
	desc.FVF = FVF;						//버퍼에 저장될 정점 규격

	//정점 버퍼 생성.
	if (DOHWA_FAILED(pB3VB->Create(desc)))
	{
		//Error...
		return DOHWA_FAIL;
	}

	IDohwaVertexBuffer9* pVB = nullptr;
	if (DOHWA_FAILED(pB3VB->QueryInterface(IID_IDohwaVertexBuffer9, (void**)&pVB)))
	{
		//...ERROR...
	}

	//외부로 리턴..
	*ppVertexBuffer = pVB;

	return DOHWA_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
// 정점 버퍼를 디바이스에 등록.  
// 
// <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
// 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.  
//
//  GraphicsEngine.pVB[StreamNumber] = pVB; 
//
// <Dohwa> 1개의 정점 버퍼만 사용합니다. 
// 
// 
// StreamNumber	(기하, 정점)스트리밍 색인 (Yena 기본값은 0)
// pVB				렌더링 할 정점 버퍼 포인터.
// OffsetBytes		(기하,정점) 스트리밍 시작 색인 (Yena 기본값은 0)
// Stride			렌더링 할 버퍼의 1마디(정점 데이터) 크기
// 
// note <Dohwa> 주요 렌더링 작업은 Dohwa GraphicsEngine 에서 처리합니다.
//
int DohwaDevice9::SetStreamSource(UINT StreamNumber, IDohwaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride)
{
	if (DOHWA_INVALIED(pVB)) return DOHWA_FAIL;

	// <Dohwa> 1개의 정점 버퍼만 사용합니다. 
	// 그래픽엔진에 설정 : 바인딩 Binding 
	m_pGraphics->SetVertexBuffer(pVB, Stride);	//★


	// 이하 생략.. <Yena> 미지원
	// ... = StreamNumber
	// ... = OffsetBytes

	return DOHWA_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
// 정점 규격을 설정합니다. 
// 
// note 렌더링시 장치에 설정된 정점버퍼 내 정점과 동일 규격을 명시해야 합니다.
//		그렇지 않다면 정상적인 렌더링 결과를 기대할 수 없습니다.
// 
//
int DohwaDevice9::SetFVF(DWORD fvf)
{
	m_pGraphics->SetFVF(fvf);	//★

	return DOHWA_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
// 렌더링 작업을 시작합니다.  
// 
// 이 메소드의 호출 이전에 정점버퍼, FVF, 마디 크기(Stride) 등 렌더링에 필요한
// 모든 정보가 디바이스에 등록되어 있어야 합니다.  
// 
// PrimitiveType	렌더링 기하 타입.
// StartVertex		렌더링 정점 시작번호 (정점 버퍼 안)
// PrimitiveCount	렌더링 기하 개수.
//
// note <Dohwa> 주요 렌더링 작업은 Dohwaw GraphicsEngine 에서 처리합니다.
//
//
int DohwaDevice9::DrawPrimitive(DOHWAPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	if (DOHWA_INVALIED(m_pGraphics->m_input.pVB)) return DOHWA_FAIL;
	if (DOHWA_INVALIED(m_pGraphics->m_input.Stride)) return DOHWA_FAIL;

	//------------------------------------------------------------
	// 렌더링 준비
	// 렌더링 전 디바이스에 공급된 개별 데이터를 재구성하여
	// 렌더링 각 단계별 반복적으로 계산되는 상수 데이터를 
	// 미리 계산합니다.  파이프라인의 연산 속도 개선이 목적.
	//------------------------------------------------------------
	//------------------------------------------------------------
	// 그래픽 엔진 정보 구성
	//------------------------------------------------------------ 
	m_pGraphics->PreDraw(PrimitiveType, StartVertex, PrimitiveCount);


	//------------------------------------------------------------
	// 렌더링 시작
	// 각 렌더링 파이프라인 단계별 적절한 연산이 수행되며
	// 그 결과를 다음 파이프라인으로 공급합니다.★
	// Yena 는 각 기능을 가능한 상세하게 구현하겠습니다.
	// 
	// 각 변환에 필요한 데이터는 DrawPrimitive 함수가 호출되기 전에
	// Device 에 공급되어야 합니다. 
	//------------------------------------------------------------
	m_pGraphics->Draw(); //★


	//------------------------------------------------------------
	// 렌더링 종료 - 파이프라인 종료 후처리 
	// 임시 버퍼 해제, 디버깅 정보 정리 등...
	//------------------------------------------------------------
	m_pGraphics->PostDraw();


	return DOHWA_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 조절.
//
int DohwaDevice9::SetRenderState(DOHWARENDERSTATETYPE state, DWORD value)
{
	m_pGraphics->SetRenderState(state, value);
	return DOHWA_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태 얻기   
//
int DohwaDevice9::GetRenderState(DOHWARENDERSTATETYPE state, DWORD* value)
{
	m_pGraphics->GetRenderState(state, value);

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 변환 행렬 설정
//
int DohwaDevice9::SetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* m) //★
{
	m_pGraphics->SetTransform(ts, m);
	//m_mTFM[ts] = *m;

	return DOHWA_OK;

}

///////////////////////////////////////////////////////////////////////////////
//
// 변환 행렬 얻기
//
int DohwaDevice9::GetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* m) // ★ 
{
	m_pGraphics->GetTransform(ts, m);
	//*m = m_mTFM[ts];

	return DOHWA_OK;
}



////////////////////////////////////////////////////////////////////////////////
//
// DohwaDevice9 참조 관련 메소드
// 
ULONG DohwaDevice9::AddRef(void)
{
	return ++m_ref;
};

ULONG DohwaDevice9::Release(void)
{
	if (--m_ref <= 0) delete this;

	return m_ref;
};

int DohwaDevice9::QueryInterface(DH_IID riid, _out_ void** ppvObject)
{
	if (DHIsEqualIID(riid, IID_IDohwaDevice9))
	{
		AddRef();
		*ppvObject = dynamic_cast<IDohwaDevice9*>(this);
	}

	return DOHWA_OK;
}