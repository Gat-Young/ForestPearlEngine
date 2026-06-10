#include "Mia.h"

////////////////////////////////
// Mia SWR 구현 클래스 선언 : B3Class / B3xxx 클래스 전용, 외부 노출 방지용
#include "Define/mcB3Class.h"
#include "VertexBuffer/mcB3VertexBuffer.h"
#include "RenderingDevice/mcB3Graphics.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// B3MiaGraphicsEngine 데이터 접근 재정의
//
#define m_pVB				m_pGraphics->m_input.pVB
#define m_FVF				m_pGraphics->m_input.FVF
#define m_Stride			m_pGraphics->m_input.Stride
#define m_TopologyVtxSize	m_pGraphics->m_input.TopologyVtxSize

#define m_RState			m_pGraphics->m_pso.RState

#define m_PresentParam		m_pGraphics->m_om.PresentParam

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 각종 렌더링 '디바이스 Device' 들을 관리할 최상위 클래스
// : Direct3D 대응
//
class B3Mia : public IMia
{
public:
    B3Mia(void);
    virtual ~B3Mia(void);

////////////////////////////////
// 인터페이스 오버라이드
public:
    virtual int CreateDevice(HWND hwnd, MIAPRESENT_PARAMETERS* pp, DWORD vp, LPMIADEVICE9* pDev) override;

////////////////////////////////
//참조 카운트 메소드 재정의
public:
    virtual ULONG AddRef(void) override;
    virtual ULONG Release(void) override;
    virtual int QueryInterface(MC_IID riid, _out_ void** ppvObject) override;

protected:
    // 레퍼런스 참조 카운트
    ULONG m_ref = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DX9 전용 렌더링 "디바이스" 클래스,
// : IDirect3DDevice9 대응
//
class B3MiaDevice9 : public IMiaDevice9
{
    friend class B3Mia;

public:
    B3MiaDevice9(void);
    virtual ~B3MiaDevice9(void);

    ////////////////////////////////
    // 장치 운용 기본 정보
protected:
    HWND	m_hWnd;
    DWORD m_VertexProcessing;

    ////////////////////////////////
    // 렌더타겟(Back-Buffer) 구성용 핸들
protected:
    HDC  m_hRT;
    COLORREF  m_BkColor;
    #define m_hSurfaceRT m_hRT

    ////////////////////////////////
    // 렌더링 그래픽스 엔진
protected:
    B3MiaGraphicsEngine9* m_pGraphics;
    #define m_pGX	m_pGraphics

protected:
    // 레퍼런스 참조 카운트
    ULONG m_ref = 0;

/////////////////////////////////////////////////////////////
//
// 내부 메소드
//
protected:
    int _Create(HWND hwnd, MIAPRESENT_PARAMETERS* pp, DWORD vp);

/////////////////////////////////////////////////////////////
//
// 인터페이스 재정의
//
    ////////////////////////////////
    // 스왑체인 및 렌더타겟 운용 메소드
public:
    virtual int  BeginScene() override;
    virtual int  EndScene() override;
    virtual int  Clear(COLORREF color) override;
    virtual int  Present() override;

    ////////////////////////////////
    // 기하 버퍼 및 렌더링 메소드
    virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3MPOOL Pool, _out_ IMiaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle) override;
    virtual int SetStreamSource(UINT SteamNumber, IMiaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride) override;
    virtual int SetFVF(DWORD FVF) override;
    virtual int DrawPrimitive(B3MPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) override;

    ////////////////////////////////
    // 렌더링 상태 조절 메소드
    virtual int SetRenderState(B3MRENDERSTATETYPE State, DWORD Value) override;
    virtual int GetRenderState(B3MRENDERSTATETYPE State, DWORD* pValue) override;

    ////////////////////////////////
    // 멤버데이터 접근자 Accessors
public:
    virtual HDC	 GetRT() override;
    virtual COLORREF  GetBkColor() override;

    ////////////////////////////////
    //참조 카운트 메소드 재정의
public:
    virtual ULONG AddRef(void) override;
    virtual ULONG Release(void) override;
    virtual int QueryInterface(MC_IID riid, _out_ void** ppvObject) override;

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 함수 구현부
//
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// YenaCreate9
//
IMia* MiaCreate9(DWORD ver)
{
    // COM 객체 생성
    B3Mia* pB3Mia = new B3Mia;
    assert(pB3Mia != NULL);

    // 인터페이스 생성 (RTTI)
    IMia* pMia = nullptr;
    //인터페이스 질의 및 획득 (GUID 및 Query)
    if (MC_FAILED(pB3Mia->QueryInterface(IID_IMia, (void**)&pMia)))
    {
        // ERROR
    }

    // 객체 참조카운트 증가
    //pB3Mia->AddRef();

    return pMia;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// class B3Mia
//
B3Mia::B3Mia(void)
{
#ifdef _DEBUG
    MIA::mcLog(_T("B3Mia 생성됨!"));
#endif
}

B3Mia::~B3Mia(void)
{
#ifdef _DEBUG
    MIA::mcLog(_T("B3Mia 제거됨"));
#endif
}

int B3Mia::CreateDevice(HWND hwnd, MIAPRESENT_PARAMETERS* pp, DWORD vp, LPMIADEVICE9* pDev)
{
    if (hwnd == NULL) return MC_FAIL;
    if (pp == NULL) return MC_FAIL;
    if (pDev == NULL) return MC_FAIL;

    B3MiaDevice9* pB3Dev = new B3MiaDevice9;
    assert(pB3Dev != NULL);
    pB3Dev->_Create(hwnd, pp, vp);

    //pB3Dev->m_hWnd = hwnd;
    //pB3Dev->m_PresentParam = *pp;
    //pB3Dev->m_VertexProcessing = vp;
    //pB3Dev->_RenderTargetCreate();

    // 인터페이스 생성 (RTTI)
    //IMiaDevice9* ppDevice = dynamic_cast<IMiaDevice9*>(pB3Dev);
    //assert(ppDevice != NULL);

    IMiaDevice9* ppDevice = nullptr;
    if (MC_FAILED(pB3Dev->QueryInterface(IID_IMiaDevice9, (void**)&ppDevice)))
    {
        // ERROR
    }

    // 객체 참조카운트 증가
    //pB3Dev->AddRef();

    *pDev = ppDevice;  //외부에 리턴

    return MC_OK;
}

ULONG B3Mia::AddRef(void)
{
    return ++m_ref;
};

ULONG B3Mia::Release(void)
{
    if (--m_ref <= 0) delete this;

    return m_ref;
};

int B3Mia::QueryInterface(MC_IID riid, _out_ void** ppvObject)
{
    if (mcIsEqualIID(riid, IID_IMia))
    {
        AddRef();
        *ppvObject = dynamic_cast<IMia*>(this);  // 지정 인터페이스 리턴
    }

    return MC_OK;
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// class B3MiaDevice9
//
B3MiaDevice9::B3MiaDevice9(void)
{
    m_hWnd = NULL;
    m_VertexProcessing = 0;

    m_hRT = NULL;
    m_BkColor = RGB(0, 0, 255);

    m_pGraphics = nullptr;

#ifdef _DEBUG
    MIA::mcLog(_T("B3MiaDevice9 생성됨!"));
#endif
}

B3MiaDevice9::~B3MiaDevice9(void)
{
    //_RenderTargetRelease();
    SafeDelete(m_pGraphics);

#ifdef _DEBUG
    MIA::mcLog(_T("B3MiaDevice9 제거됨"));
#endif
}

int B3MiaDevice9::_Create(HWND hwnd, MIAPRESENT_PARAMETERS* pp, DWORD vp)
{
    if (MC_INVALIED(hwnd)) return MC_FAIL;
    if (MC_INVALIED(pp))   return MC_FAIL;

    LPB3MIAGRAPHICSENGINE9 pGraphics = new B3MiaGraphicsEngine9;
    ASSERT(pGraphics);

    B3MiaGraphicsEngine9::OUTPUT_MERGE om = {};
    om.hWnd = hwnd;
    om.PresentParam = *pp;
    pGraphics->m_om = om;

    pGraphics->_RenderTargetCreate();

    pGraphics->SetDev(this);

    m_hWnd = hwnd;
    m_VertexProcessing = vp;
    m_pGraphics = pGraphics;
    m_hSurfaceRT = pGraphics->_GetDCRT();
    m_BkColor = B3MXCOLOR(0.25f, 0.25f, 0.25f, 1);

    return MC_OK;
}

int B3MiaDevice9::BeginScene()
{
    m_pGraphics->_BeginScene();
    /*
    SetBkMode(m_hSurfaceRT, TRANSPARENT);

    //기본 펜 색상 지정
    HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
    SelectObject(m_hSurfaceRT, hPen);
    */

    return MC_OK;
}

int B3MiaDevice9::EndScene()
{
    m_pGraphics->_EndScene();

    return MC_OK;
}

////////////////////////////////
// 렌더 타겟 클리어
int B3MiaDevice9::Clear(COLORREF color)
{
    /*
    HBRUSH hBrush = CreateSolidBrush(color);
    RECT rc = { 0, 0, (LONG)m_PresentParam.Width,  (LONG)m_PresentParam.Height };
    FillRect(m_hSurfaceRT, &rc, hBrush);
    DeleteObject(hBrush);
    */

    m_BkColor = color;
    m_pGraphics->_Clear(color);

    return MC_OK;
}

///////////////////////////////////
// 장면 출력
int B3MiaDevice9::Present()
{
    /*
    HDC hdc = GetDC(m_hWnd);
    BitBlt(hdc, 0, 0, m_PresentParam.Width, m_PresentParam.Height, m_hSurfaceRT, 0, 0, SRCCOPY);
    ReleaseDC(m_hWnd, hdc);
    */

    return m_pGraphics->_Present();
}

int B3MiaDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3MPOOL Pool, _out_ IMiaVertexBuffer9** ppVertexBuffer, _in_opt_ HANDLE* pSharedHandle)
{
    //정점 버퍼 운용 개체 생성.
    LPB3MIAVERTEXBUFFER9 pB3VB = new B3MiaVertexBuffer9;
    if (MC_INVALIED(pB3VB)) return MC_FAIL;

    //생성할 정점 버퍼 정보 기술
    //<Mia> 시스템 메모리를 사용하므로 필수 동작은 아니지만, 구조의 일관성을 위해 사용.
    //GPU 메모리(VRAM) 확보를 위해서 이 정도의 수고는 필요하다...정도로 이해해 봅시다.
    B3MVERTEXBUFFER_DESC desc = {};
    desc.Format = B3MFMT_VERTEXDATA;		//버퍼 포멧 : 정점 데이터용
    desc.Type = B3MRTYPE_VERTEXBUFFER;	//버펴 형식 : 정점 버퍼
    desc.Usage = B3MUSAGE_WRITEONLY;		//버퍼 사용방식 : 읽기 전용, <주> 정점버퍼는 CPU "쓰기전용", GPU "읽기 전용" 임을 상기합시다.
    desc.Pool = Pool;						//버퍼 메모리 풀 형식
    desc.Size = Length;					//버퍼 크기
    desc.FVF = FVF;						//버퍼에 저장될 정점 규격

    //정점 버퍼 생성.
    if (MC_FAILED(pB3VB->_Create(desc)))
    {
        return MC_FAIL;
    }

    IMiaVertexBuffer9* pVB = nullptr;
    if (MC_FAILED(pB3VB->QueryInterface(IID_IMiaVertexBuffer9, (void**)&pVB)))
    {
        // ERROR
    }

    *ppVertexBuffer = pVB;
    return MC_OK;
}

int B3MiaDevice9::SetStreamSource(UINT SteamNumber, IMiaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride)
{
    if (MC_INVALIED(pVB))
        return MC_FAIL;

    //m_pVB[0] = pVB;
    //m_Stride = Stride;
    m_pGraphics->_SetVertexBuffer(pVB, Stride);

    return MC_OK;
}

int B3MiaDevice9::SetFVF(DWORD FVF)
{
    //m_FVF = FVF;
    m_pGraphics->_SetFVF(FVF);

    return MC_OK;
}

int B3MiaDevice9::DrawPrimitive(B3MPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    if (MC_INVALIED(m_pVB))
        return MC_FAIL;
    if (MC_INVALIED(m_Stride))
        return MC_FAIL;

    /*
    m_PrimCnt = PrimitiveCount;
    m_StartVtx = StartVertex;

    _VertexPipeLine();
    _GeometryPipeLine();
    _PixelPipeLine();
    */

    m_pGraphics->PreDraw(PrimitiveType, StartVertex, PrimitiveCount);
    m_pGraphics->Draw();
    m_pGraphics->PostDraw();

    return MC_OK;
}

int B3MiaDevice9::SetRenderState(B3MRENDERSTATETYPE State, DWORD Value)
{
    //m_RState[State] = Value;
    m_pGraphics->_SetRenderState(State, Value);

    return MC_OK;
}

int B3MiaDevice9::GetRenderState(B3MRENDERSTATETYPE State, DWORD* pValue)
{
    //pValue = m_RState[State];
    m_pGraphics->_GetRenderState(State, pValue);

    return MC_OK;
}

HDC B3MiaDevice9::GetRT()
{
    m_hSurfaceRT = m_pGraphics->_GetDCRT();
    return m_hSurfaceRT;
}

COLORREF B3MiaDevice9::GetBkColor()
{
    return m_BkColor;
}

ULONG B3MiaDevice9::AddRef(void)
{
    return ++m_ref;
};

ULONG B3MiaDevice9::Release(void)
{
    if (--m_ref <= 0) delete this;

    return m_ref;
};

int B3MiaDevice9::QueryInterface(MC_IID riid, _out_ void** ppvObject)
{
    if (mcIsEqualIID(riid, IID_IMiaDevice9))
    {
        AddRef();
        *ppvObject = dynamic_cast<IMiaDevice9*>(this);  // 지정 인터페이스 리턴
    }

    return MC_OK;
}


///////////////////////////////////
// B3MiaGraphicsEngine 의 정의와 구별용 정의 해제
#undef m_pVB
#undef m_FVF
#undef m_Stride
#undef m_TopologyVtxSize
#undef m_RState
#undef m_PresentParam