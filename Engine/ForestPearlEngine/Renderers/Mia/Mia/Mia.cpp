#include "Mia.h"
#include "mcMath.h"

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
    MIAPRESENT_PARAMETERS m_PresentParam;
    DWORD m_VertexProcessing;

    ////////////////////////////////
    // 렌더타겟(Back-Buffer) 구성용 핸들
protected:
    HBITMAP  m_hBmpRT;
    HDC  m_hSurfaceRT;
    COLORREF  m_BkColor;

    ////////////////////////////////
    // 정점 버퍼 관련 데이터
protected:
    IMiaVertexBuffer9* m_pVB[1];	// 정점 버퍼 (인터페이스)
    DWORD m_FVF;						// 정점 규격
    UINT  m_Stride;                       // 정점 1마디 크기

    ////////////////////////////////
    // 렌더링 옵션
protected:
    UINT m_PrimCnt;		// Face 개수
    UINT m_StartVtx;		// 시작 정점 번호
    UINT  m_VtxNum;						// 현재 작업중인 정점 번호 (디버깅용)
    UINT  m_FaceNum;					// 현재 작업중인 페이스 번호 (디버깅용)

    // 렌더링 상태값
    DWORD  m_RState[B3MRS_MAX_];

protected:
    // 레퍼런스 참조 카운트
    ULONG m_ref = 0;

/////////////////////////////////////////////////////////////
//
// 내부 메소드
//
protected:
    int _RenderTargetCreate();
    void _RenderTargetRelease();

    // 렌더링 파이프 라인 별 연산 메소드
    int _VertexPipeLine();		    // 정점 파이프 라인
    int _GeometryPipeLine();		// 기하 파이프 라인
    int _PixelPipeLine();		        // 픽셀 파이프 라인

    //// 그리기 함수
    //Line
    //int _DrawLine(B3MXVECTOR2 v0, B3MXVECTOR2 v1);
    int _DrawLine(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXCOLOR c0, B3MXCOLOR c1);
    void _Bresenham(LONG h, LONG w, LONG pW, LONG pH, LONG endW, int AddW, BOOL isWX, B3MXCOLOR Sc, B3MXCOLOR Ec);
    void _DrawHorizontal(LONG sX, LONG eX, LONG Y, B3MXCOLOR c0, B3MXCOLOR c1);
    void _DrawVertical(LONG sY, LONG eY, LONG X, B3MXCOLOR c0, B3MXCOLOR c1);
    //Face
    //int _DrawFace(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2);
    int _DrawFace(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2, B3MXCOLOR c0, B3MXCOLOR c1, B3MXCOLOR c2);
    LONGLONG _Dot(POINT V1, POINT V2);

    // 렌더링 상태 초기화
    void _InitRenderState();
    bool _FaceCulling(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2);

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
    virtual int CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3MPOOL Pool, _out_ IMiaVertexBuffer9** ppVB, _in_opt_ HANDLE* pSharedHandle) override;
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

#define CHECKSTATE( state, val ) (m_RState[(state)] == (val))



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 정점 버퍼 구현 클래스
// : IDirect3DVertexBuffer9 대응
//
class B3MiaVertexBuffer9 : public IMiaVertexBuffer9
{
    friend class B3MiaDevice9;

public:
    B3MiaVertexBuffer9(void);
    virtual ~B3MiaVertexBuffer9(void);

protected:
    // 레퍼런스 참조 카운트
    ULONG m_ref = 0;

protected:
    void* m_pVBuffer;				    // 정점 버퍼 : 실제 데이터가 저장됨
    DWORD m_SizeInByte;			//정점 버퍼 크기
    DWORD m_FVF;					// 정점 버퍼 규격 조합 플래그
    UINT  m_Stride;					// 정점 버퍼 안의 1마디 크기
    BOOL  m_bLocked;				// 버퍼 잠금 상태
    B3MVERTEXBUFFER_DESC m_Desc;	// 정점 버퍼 정보

    /////////////////////////////////////////////////////////////
    //
    // 내부 메소드
    //
protected:
    int		_Create(B3MVERTEXBUFFER_DESC desc);
    //int		_Create(UINT Length, DWORD FVF, B3MPOOL Pool);		//구형 호환

    void* _GetVBuffer() { return m_pVBuffer; }
    DWORD	_GetSizeInByte() { return m_SizeInByte; }
    DWORD	_GetFVF() { return m_FVF; }
    DWORD	_GetStride() { return m_Stride; }


    /////////////////////////////////////////////////////////////
    //
    // 인터페이스 재정의
    //
protected:
    virtual int   Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags) override;
    virtual int   Unlock(void) override;
    virtual int   GetDesc(_out_ B3MVERTEXBUFFER_DESC* pDesc) override;

    virtual void* GetBuffer(void) override;
    virtual UINT  GetVertexCount(void) override;
    virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData) override;

    virtual ULONG AddRef(void) override;
    virtual ULONG Release(void) override;
    virtual int   QueryInterface(MC_IID riid, _out_ void** ppvObject) override;
};

typedef B3MiaVertexBuffer9* LPB3MIAVERTEXBUFFER9;




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

    pB3Dev->m_hWnd = hwnd;
    pB3Dev->m_PresentParam = *pp;
    pB3Dev->m_VertexProcessing = vp;
    pB3Dev->_RenderTargetCreate();

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
    ZeroMemory(&m_PresentParam, sizeof(m_PresentParam));

    m_hBmpRT = NULL;
    m_hSurfaceRT = NULL;
    m_BkColor = RGB(0, 0, 255);

    m_pVB[0] = NULL;
    m_FVF = 0;
    m_Stride = 0;

    m_PrimCnt = 0;
    m_StartVtx = 0;

    // 렌더상태 초기화
    _InitRenderState();

#ifdef _DEBUG
    MIA::mcLog(_T("B3MiaDevice9 생성됨!"));
#endif
}

B3MiaDevice9::~B3MiaDevice9(void)
{
    _RenderTargetRelease();

#ifdef _DEBUG
    MIA::mcLog(_T("B3MiaDevice9 제거됨"));
#endif
}

///////////////////////////////////
// 백버퍼용 렌더타겟을 생성
int B3MiaDevice9::_RenderTargetCreate()
{
    HDC hdc = GetDC(m_hWnd);

    m_hSurfaceRT = CreateCompatibleDC(hdc);
    m_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, m_PresentParam.Width, m_PresentParam.Height);
    SelectObject(m_hSurfaceRT, m_hBmpRT);

    ReleaseDC(m_hWnd, hdc);

    return MC_OK;
}

///////////////////////////////////
// 렌더타겟 제거
void B3MiaDevice9::_RenderTargetRelease()
{
    DeleteObject(m_hBmpRT);
    DeleteDC(m_hSurfaceRT);
}

int B3MiaDevice9::_VertexPipeLine()
{
    return MC_OK;
}

int B3MiaDevice9::_GeometryPipeLine()
{
    return MC_OK;
}

int B3MiaDevice9::_PixelPipeLine()
{
    B3MVERTEXBUFFER_DESC desc;
    m_pVB[0]->GetDesc(&desc);

    UINT  vtxCnt = desc.Size / m_Stride;
    void* pOrgVB = m_pVB[0]->GetBuffer();

    BYTE* pCurrVB = (BYTE*)pOrgVB;
    BYTE* pCurrVtx = NULL;
    UINT  faceCnt = 0;

    for (UINT i = m_StartVtx; i < vtxCnt; i += 3)
    {
        pCurrVB = (BYTE*)pOrgVB + (i)*m_Stride;

        pCurrVtx = pCurrVB;
        B3MVECTOR2  v0 = *(B3MVECTOR2*)pCurrVtx;
        DWORD Dc0 = *(DWORD*)(pCurrVtx + sizeof(B3MVECTOR2));
        B3MXCOLOR c0 = B3MXCOLOR(Dc0);

        pCurrVtx = pCurrVB + m_Stride;
        B3MVECTOR2  v1 = *(B3MVECTOR2*)pCurrVtx;
        DWORD Dc1 = *(DWORD*)(pCurrVtx + sizeof(B3MVECTOR2));
        B3MXCOLOR c1 = B3MXCOLOR(Dc1);

        pCurrVtx = pCurrVB + (2) * m_Stride;
        B3MVECTOR2  v2 = *(B3MVECTOR2*)pCurrVtx;
        DWORD Dc2 = *(DWORD*)(pCurrVtx + sizeof(B3MVECTOR2));
        B3MXCOLOR c2 = B3MXCOLOR(Dc2);

        if (_FaceCulling(v0, v1, v2))
        {
            //true 가 리턴되면, 컬링해야 하는 삼각형으로 판정, 다음 삼각형으로 넘어간다.
            //....

            //지정 개수 이상의 삼각형이 그려지면, 작업 종료... 			
            if (++faceCnt >= m_PrimCnt)
                break;
            else
                continue;
        }

        if (m_RState[B3MRS_FILLMODE] == B3MFILL_WIREFRAME)
        {
            // 라인 그리기
            _DrawLine(v0, v1, c0, c1);
            _DrawLine(v0, v2, c0, c2);
            _DrawLine(v1, v2, c1, c2);
        }
        else
        {
            // 삼각형 그리기
            _DrawFace(v0, v1, v2, c0, c1, c2);
        }

        if (++faceCnt >= m_PrimCnt)
            break;
    }

    return MC_OK;
}

//int B3MiaDevice9::_DrawLine(B3MXVECTOR2 v0, B3MXVECTOR2 v1)
//{
//    B3MVECTOR2 sp = v0;
//    B3MVECTOR2 ep = v1;
//
//    if (ep.y == sp.y)
//    {
//        if (sp.x > ep.x)
//        {
//            _DrawHorizontal(ep.x, sp.x, sp.y);
//        }
//        else
//        {
//            _DrawHorizontal(sp.x, ep.x, sp.y);
//        }
//    }
//    else if (ep.x == sp.x)
//    {
//        if (sp.y > ep.y)
//        {
//            _DrawVertical(ep.y, sp.y, sp.x);
//        }
//        else
//        {
//            _DrawVertical(sp.y, ep.y, sp.x);
//        }
//    }
//
//    float a = float(ep.y - sp.y) / (ep.x - sp.x);
//
//    if (a > 1)
//    {
//    	if (ep.x - sp.x > 0) //2팔분면
//    	{
//    		LONG w = ep.y - sp.y;
//    		LONG h = ep.x - sp.x;
//
//    		_Bresenham(h, w, sp.y, sp.x, ep.y, 1, FALSE);
//    	}
//    	else  //6팔분면
//    	{
//    		LONG w = sp.y - ep.y;
//    		LONG h = sp.x - ep.x;
//
//    		_Bresenham(h, w, ep.y, ep.x, sp.y, 1, FALSE);
//    	}
//    }
//    else if (a > 0)
//    {
//    	if (ep.x - sp.x > 0)  //1팔분면
//    	{
//    		LONG w = ep.x - sp.x;
//    		LONG h = ep.y - sp.y;
//
//    		_Bresenham(h, w, sp.x, sp.y, ep.x, 1, TRUE);
//    	}
//    	else  //5팔분면
//    	{
//    		LONG w = sp.x - ep.x;
//    		LONG h = sp.y - ep.y;
//
//    		_Bresenham(h, w, ep.x, ep.y, sp.x, 1, TRUE);
//    	}
//    }
//    else if (a > -1)
//    {
//    	if (ep.x - sp.x > 0)  //8팔분면
//    	{
//    		LONG w = ep.x - sp.x;
//    		LONG h = sp.y - ep.y;
//
//    		_Bresenham(h, w, ep.x, ep.y, sp.x, -1, TRUE);
//    	}
//    	else  //4팔분면
//    	{
//    		LONG w = sp.x - ep.x;
//    		LONG h = ep.y - sp.y;
//
//    		_Bresenham(h, w, sp.x, sp.y, ep.x, -1, TRUE);
//    	}
//    }
//    else
//    {
//    	if (ep.x - sp.x > 0)  //7팔분면
//    	{
//    		LONG w = sp.y - ep.y;
//    		LONG h = ep.x - sp.x;
//
//    		_Bresenham(h, w, sp.y, sp.x, ep.y, -1, FALSE);
//    	}
//    	else  //3팔분면
//    	{
//    		LONG w = ep.y - sp.y;
//    		LONG h = sp.x - ep.x;
//
//    		_Bresenham(h, w, ep.y, ep.x, sp.y, -1, FALSE);
//    	}
//    }
//
//    return MC_OK;
//}

///////////////////////////////////
// 점 2개 라인 그리기
int B3MiaDevice9::_DrawLine(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXCOLOR c0, B3MXCOLOR c1)
{
    B3MVECTOR2 sp = v0;
    B3MVECTOR2 ep = v1;

    if (ep.y == sp.y)
    {
        if (sp.x > ep.x)
        {
            _DrawHorizontal(ep.x, sp.x, sp.y, c1, c0);
        }
        else
        {
            _DrawHorizontal(sp.x, ep.x, sp.y, c0, c1);
        }
    }
    else if (ep.x == sp.x)
    {
        if (sp.y > ep.y)
        {
            _DrawVertical(ep.y, sp.y, sp.x, c1, c0);
        }
        else
        {
            _DrawVertical(sp.y, ep.y, sp.x, c0, c1);
        }
    }

    float a = float(ep.y - sp.y) / (ep.x - sp.x);

    if (a > 1)
    {
        if (ep.x - sp.x > 0) //2팔분면
        {
            LONG w = ep.y - sp.y;
            LONG h = ep.x - sp.x;

            _Bresenham(h, w, sp.y, sp.x, ep.y, 1, FALSE, c0, c1);
        }
        else  //6팔분면
        {
            LONG w = sp.y - ep.y;
            LONG h = sp.x - ep.x;

            _Bresenham(h, w, ep.y, ep.x, sp.y, 1, FALSE, c1, c0);
        }
    }
    else if (a > 0)
    {
        if (ep.x - sp.x > 0)  //1팔분면
        {
            LONG w = ep.x - sp.x;
            LONG h = ep.y - sp.y;

            _Bresenham(h, w, sp.x, sp.y, ep.x, 1, TRUE, c0, c1);
        }
        else  //5팔분면
        {
            LONG w = sp.x - ep.x;
            LONG h = sp.y - ep.y;

            _Bresenham(h, w, ep.x, ep.y, sp.x, 1, TRUE, c1, c0);
        }
    }
    else if (a > -1)
    {
        if (ep.x - sp.x > 0)  //8팔분면
        {
            LONG w = ep.x - sp.x;
            LONG h = sp.y - ep.y;

            _Bresenham(h, w, ep.x, ep.y, sp.x, -1, TRUE, c1, c0);
        }
        else  //4팔분면
        {
            LONG w = sp.x - ep.x;
            LONG h = ep.y - sp.y;

            _Bresenham(h, w, sp.x, sp.y, ep.x, -1, TRUE, c0, c1);
        }
    }
    else
    {
        if (ep.x - sp.x > 0)  //7팔분면
        {
            LONG w = sp.y - ep.y;
            LONG h = ep.x - sp.x;

            _Bresenham(h, w, sp.y, sp.x, ep.y, -1, FALSE, c0, c1);
        }
        else  //3팔분면
        {
            LONG w = ep.y - sp.y;
            LONG h = sp.x - ep.x;

            _Bresenham(h, w, ep.y, ep.x, sp.y, -1, FALSE, c1, c0);
        }
    }

    return MC_OK;
}

void B3MiaDevice9::_Bresenham(LONG h, LONG w, LONG pW, LONG pH, LONG endW, int AddW, BOOL isWX, B3MXCOLOR Sc, B3MXCOLOR Ec)
{
    LONG d = 2 * h - w;
    const int dashGap = 5;
    int dashPixelCount = -4;
    COLORREF PixelColor;
    float Alpha;
    LONG CW = pW;

    while (CW * AddW < endW * AddW)
    {
        Alpha = (float)(CW - pW) / (float)(endW - pW);

        MCLerp(&PixelColor, (DWORD)Sc, (DWORD)Ec, Alpha);

        if (isWX)
        {
            SetPixel(m_hSurfaceRT, CW, pH, PixelColor);
        }
        else
        {
            SetPixel(m_hSurfaceRT, pH, CW, PixelColor);
        }

    	if (d > 0)
    	{
    		d -= 2 * w;
    		pH += 1;
    	}
    	d += 2 * h;
        CW += AddW;
    }
}

void B3MiaDevice9::_DrawHorizontal(LONG sX, LONG eX, LONG Y, B3MXCOLOR cS, B3MXCOLOR cE)
{
    COLORREF PixelColor;

    for (LONG x = sX; x <= eX; x++)
    {
        MCLerp(&PixelColor, (DWORD)cS, (DWORD)cE, (float)(x - sX) / (eX - sX));
        SetPixel(m_hSurfaceRT, x, Y, PixelColor);
    }
}

void B3MiaDevice9::_DrawVertical(LONG sY, LONG eY, LONG X, B3MXCOLOR cS, B3MXCOLOR cE)
{
    COLORREF PixelColor;

    for (LONG y = sY; y <= eY; y++)
    {
        MCLerp(&PixelColor, (DWORD)cS, (DWORD)cE, (float)(y - sY) / (eY - sY));
        SetPixel(m_hSurfaceRT, X, y, PixelColor);
    }
}

//int B3MiaDevice9::_DrawFace(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2)
//{
//    ////////////////////////////////
//    // 순회 시작, 종료점 계산
//
//    B3MVECTOR2 Vtx[3] = { v0, v1 ,v2 };
//
//    LONG startX = Vtx[0].x;
//    LONG endX = Vtx[0].x;
//    LONG startY = Vtx[0].y;
//    LONG endY = Vtx[0].y;
//
//    for (int i = 1; i < 3; i++)
//    {
//        if (Vtx[i].x < startX)
//        {
//            startX = Vtx[i].x;
//        }
//        if (Vtx[i].x > endX)
//        {
//            endX = Vtx[i].x;
//        }
//
//        if (Vtx[i].y < startY)
//        {
//            startY = Vtx[i].y;
//        }
//        if (Vtx[i].y > endY)
//        {
//            endY = Vtx[i].y;
//        }
//    }
//
//    ////////////////////////////////
//    // 벡터 계산
//    POINT Vu = { Vtx[0].x - Vtx[2].x, Vtx[0].y - Vtx[2].y };
//    POINT Vv = { Vtx[1].x - Vtx[2].x, Vtx[1].y - Vtx[2].y };
//
//    ////////////////////////////////
//    // 펜 색 지정
//    //HPEN currentPen = (HPEN)GetCurrentObject(g_hRT, OBJ_PEN);
//    //LOGPEN penProperty;
//    //GetObject(currentPen, sizeof(LOGPEN), &penProperty);
//    //COLORREF penColor = penProperty.lopnColor;
//    COLORREF P1Color = RGB(255, 0, 0);
//    COLORREF P2Color = RGB(0, 255, 0);
//    COLORREF P3Color = RGB(0, 0, 255);
//
//    ////////////////////////////////
//    // 순회하며 그리기
//    for (LONG Y = startY; Y <= endY; Y++)
//    {
//        for (LONG X = startX; X <= endX; X++)
//        {
//            POINT Vw = { X - Vtx[2].x, Y - Vtx[2].y };
//
//            float S = (float)(_Dot(Vw, Vv) * _Dot(Vu, Vv) - _Dot(Vw, Vu) * _Dot(Vv, Vv)) / (float)(_Dot(Vu, Vv) * _Dot(Vu, Vv) - _Dot(Vu, Vu) * _Dot(Vv, Vv));
//            float T = (float)(_Dot(Vw, Vu) * _Dot(Vu, Vv) - _Dot(Vw, Vv) * _Dot(Vu, Vu)) / (float)(_Dot(Vu, Vv) * _Dot(Vu, Vv) - _Dot(Vu, Vu) * _Dot(Vv, Vv));
//            float R = 1 - S - T;
//
//            bool IsSInRange = (S >= 0 && S <= 1);
//            bool IsTInRange = (T >= 0 && T <= 1);
//            bool IsRInRange = (R >= 0 && R <= 1);
//
//            if (!(IsSInRange && IsTInRange && IsRInRange))
//            {
//                continue;
//            }
//
//            BYTE ColorR = GetRValue(P1Color) * S + GetRValue(P2Color) * T + GetRValue(P3Color) * R;
//            BYTE ColorG = GetGValue(P1Color) * S + GetGValue(P2Color) * T + GetGValue(P3Color) * R;
//            BYTE ColorB = GetBValue(P1Color) * S + GetBValue(P2Color) * T + GetBValue(P3Color) * R;
//            COLORREF PixelColor = RGB(ColorR, ColorG, ColorB);
//            SetPixel(m_hSurfaceRT, X, Y, PixelColor);
//        }
//    }
//
//
//    return 0;
//}

///////////////////////////////////
// 점 3개 삼각형 그리기
int B3MiaDevice9::_DrawFace(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2, B3MXCOLOR c0, B3MXCOLOR c1, B3MXCOLOR c2)
{
    ////////////////////////////////
        // 순회 시작, 종료점 계산

    B3MVECTOR2 Vtx[3] = { v0, v1 ,v2 };

    LONG startX = Vtx[0].x;
    LONG endX = Vtx[0].x;
    LONG startY = Vtx[0].y;
    LONG endY = Vtx[0].y;

    for (int i = 1; i < 3; i++)
    {
        if (Vtx[i].x < startX)
        {
            startX = Vtx[i].x;
        }
        if (Vtx[i].x > endX)
        {
            endX = Vtx[i].x;
        }

        if (Vtx[i].y < startY)
        {
            startY = Vtx[i].y;
        }
        if (Vtx[i].y > endY)
        {
            endY = Vtx[i].y;
        }
    }

    ////////////////////////////////
    // 벡터 계산
    POINT Vu = { Vtx[0].x - Vtx[2].x, Vtx[0].y - Vtx[2].y };
    POINT Vv = { Vtx[1].x - Vtx[2].x, Vtx[1].y - Vtx[2].y };

    ////////////////////////////////
    // 순회하며 그리기
    for (LONG Y = startY; Y <= endY; Y++)
    {
        for (LONG X = startX; X <= endX; X++)
        {
            POINT Vw = { X - Vtx[2].x, Y - Vtx[2].y };

            float S = (float)(_Dot(Vw, Vv) * _Dot(Vu, Vv) - _Dot(Vw, Vu) * _Dot(Vv, Vv)) / (float)(_Dot(Vu, Vv) * _Dot(Vu, Vv) - _Dot(Vu, Vu) * _Dot(Vv, Vv));
            float T = (float)(_Dot(Vw, Vu) * _Dot(Vu, Vv) - _Dot(Vw, Vv) * _Dot(Vu, Vu)) / (float)(_Dot(Vu, Vv) * _Dot(Vu, Vv) - _Dot(Vu, Vu) * _Dot(Vv, Vv));
            float R = 1 - S - T;

            bool IsSInRange = (S >= 0 && S <= 1);
            bool IsTInRange = (T >= 0 && T <= 1);
            bool IsRInRange = (R >= 0 && R <= 1);

            if (!(IsSInRange && IsTInRange && IsRInRange))
            {
                continue;
            }

            COLORREF PixelColor = COLORREF(c0 *S + c1 * T + c2 * R);
            SetPixel(m_hSurfaceRT, X, Y, PixelColor);
        }
    }


    return MC_OK;
}

LONGLONG B3MiaDevice9::_Dot(POINT V1, POINT V2)
{
    return V1.x * V2.x + V1.y * V2.y;
}

void B3MiaDevice9::_InitRenderState()
{
    ::ZeroMemory(m_RState, sizeof(DWORD) * B3MRS_MAX_);

    m_RState[B3MRS_FILLMODE] = B3MFILL_SOLID;
    m_RState[B3MRS_CULLMODE] = B3MCULL_CCW;
}

bool B3MiaDevice9::_FaceCulling(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2)
{
    bool bCull = false;

    switch (m_RState[B3MRS_CULLMODE])
    {
        case B3MCULL_NONE:
            bCull = false;
            break;

        case B3MCULL_CW:
        {
            B3MXVECTOR2 v01 = v1 - v0;
            B3MXVECTOR2 v02 = v2 - v0;
            float z = B3MXVec2CCW(&v01, &v02);
            if (z > 0)
                bCull = true;
        }
        break;

        case B3MCULL_CCW:
        {
            B3MXVECTOR2 v01 = v1 - v0;
            B3MXVECTOR2 v02 = v2 - v0;
            float z = B3MXVec2CCW(&v01, &v02);
            if (z < 0)
                bCull = true;
        }
        break;

    }

    return bCull;
}

int B3MiaDevice9::BeginScene()
{
    SetBkMode(m_hSurfaceRT, TRANSPARENT);

    //기본 펜 색상 지정
    HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
    SelectObject(m_hSurfaceRT, hPen);

    return MC_OK;
}

int B3MiaDevice9::EndScene()
{
    return MC_OK;
}

////////////////////////////////
// 렌더 타겟 클리어
int B3MiaDevice9::Clear(COLORREF color)
{
    HBRUSH hBrush = CreateSolidBrush(color);
    RECT rc = { 0, 0, (LONG)m_PresentParam.Width,  (LONG)m_PresentParam.Height };
    FillRect(m_hSurfaceRT, &rc, hBrush);
    DeleteObject(hBrush);

    m_BkColor = color;

    return MC_OK;
}

///////////////////////////////////
// 장면 출력
int B3MiaDevice9::Present()
{
    HDC hdc = GetDC(m_hWnd);
    BitBlt(hdc, 0, 0, m_PresentParam.Width, m_PresentParam.Height, m_hSurfaceRT, 0, 0, SRCCOPY);
    ReleaseDC(m_hWnd, hdc);

    return MC_OK;
}

int B3MiaDevice9::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, B3MPOOL Pool, _out_ IMiaVertexBuffer9** ppVB, _in_opt_ HANDLE* pSharedHandle)
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

    *ppVB = pVB;
    return MC_OK;
}

int B3MiaDevice9::SetStreamSource(UINT SteamNumber, IMiaVertexBuffer9* pVB, UINT OffsetBytes, UINT Stride)
{
    if (MC_INVALIED(pVB))
        return MC_FAIL;

    m_pVB[0] = pVB;
    m_Stride = Stride;

    return MC_OK;
}

int B3MiaDevice9::SetFVF(DWORD FVF)
{
    m_FVF = FVF;

    return MC_OK;
}

int B3MiaDevice9::DrawPrimitive(B3MPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
    if (MC_INVALIED(m_pVB))
        return MC_FAIL;

    m_PrimCnt = PrimitiveCount;
    m_StartVtx = StartVertex;

    _VertexPipeLine();
    _GeometryPipeLine();
    _PixelPipeLine();

    return MC_OK;
}

int B3MiaDevice9::SetRenderState(B3MRENDERSTATETYPE State, DWORD Value)
{
    m_RState[State] = Value;

    return MC_OK;
}

int B3MiaDevice9::GetRenderState(B3MRENDERSTATETYPE State, DWORD* pValue)
{
    *pValue = m_RState[State];

    return MC_OK;
}

HDC B3MiaDevice9::GetRT()
{
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



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// class B3MiaVertexBuffer9
//

B3MiaVertexBuffer9::B3MiaVertexBuffer9(void)
{
    m_pVBuffer = NULL;
    m_SizeInByte = 0;
    m_FVF = 0;
    m_Stride = 0;
    m_bLocked = FALSE;

    ZeroMemory(&m_Desc, sizeof(m_Desc));

#ifdef _DEBUG
    MIA::mcLog(_T("B3MiaVertexBuffer9 생성됨"));
#endif
}

B3MiaVertexBuffer9::~B3MiaVertexBuffer9(void)
{
    SafeDelArray(m_pVBuffer);

#ifdef _DEBUG
    MIA::mcLog(_T("B3MiaVertexBuffer9 제거됨"));
#endif
}

int B3MiaVertexBuffer9::_Create(B3MVERTEXBUFFER_DESC desc)
{
    if (MC_VALIED(m_pVBuffer)) return MC_FAIL;

    m_pVBuffer = static_cast<void*>(new BYTE[desc.Size]);	assert(m_pVBuffer);
    m_SizeInByte = desc.Size;
    m_FVF = desc.FVF;
    m_Desc = desc;

    m_Stride = 0;
    if (CHECK(m_FVF, B3MFVF_XY))	 m_Stride += sizeof(float) * 2;
    if (CHECK(m_FVF, B3MFVF_DIFFUSE)) m_Stride += sizeof(DWORD);     //정점에 색상값이 포함되면 크기 변경

    if (m_Stride <= 0) {  /* Error!!...  */  return MC_FAIL; }

    return MC_OK;
}

int B3MiaVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
    if (MC_ENABLED(m_bLocked))
    {
        MIA::mcLog(_T("이 버퍼는 이미 잠겨 있습니다. %s=%x)"), mcToString(m_pVBuffer), m_pVBuffer);
        //return MC_FAIL;
    }

    if (MC_INVALIED(*ppbData))
        return MC_FAIL;

    BYTE* pVB = (BYTE*)m_pVBuffer + OffsetToLock;
    *ppbData = (void*)pVB;

    m_bLocked = TRUE;

    return MC_OK;
}

int B3MiaVertexBuffer9::Unlock(void)
{
    if (MC_DISABLED(m_bLocked))
    {
        MIA::mcLog(_T("이 버퍼는 이미 잠금 해제 되어 있습니다. %s=%x)"), mcToString(m_pVBuffer), m_pVBuffer);
        //return MC_FAIL;
    }

    m_bLocked = FALSE;

    return MC_OK;
}

int B3MiaVertexBuffer9::GetDesc(_out_ B3MVERTEXBUFFER_DESC* pDesc)
{
    if (MC_INVALIED(pDesc))
        return MC_FAIL;

    *pDesc = m_Desc;

    return MC_OK;
}

void* B3MiaVertexBuffer9::GetBuffer(void)
{
    if (MC_INVALIED(m_pVBuffer))
    {
        MIA::mcLog(_T(" NULL 객체를 참조하고 있습니다 : %s = NULL, Size=%d, Stride=%d"), mcToString(m_pVBuffer), m_Desc.Size, m_Stride);
    }

    return m_pVBuffer;
}

UINT B3MiaVertexBuffer9::GetVertexCount(void)
{
    if (MC_INVALIED(m_pVBuffer))
    {
        MIA::mcLog(_T(" NULL 객체를 참조하고 있습니다 : %s = NULL, Size=%d, Stride=%d"), mcToString(m_pVBuffer), m_Desc.Size, m_Stride);
    }

    if (m_Desc.Size <= 0 || m_Stride == 0)
    {
        MIA::mcLog(_T(" 미설정 객체를 참조하고 있습니다 : %s = %x , Size=%d, Stride=%d"), mcToString(m_pVBuffer), m_pVBuffer, m_Desc.Size, m_Stride);
    }

    return m_Desc.Size / m_Stride;
}

int B3MiaVertexBuffer9::GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData)
{
    void* pVB = GetBuffer();
    UINT  vtxs = GetVertexCount();

    //외부로 리턴.
    *ppBuffer = pVB;
    *pSizeData = vtxs;

    return MC_OK;
}

ULONG B3MiaVertexBuffer9::AddRef(void)
{
    return ++m_ref;
}

ULONG B3MiaVertexBuffer9::Release(void)
{
    if (--m_ref <= 0)
        delete this;

    return m_ref;
}

int B3MiaVertexBuffer9::QueryInterface(MC_IID riid, _out_ void** ppvObject)
{
    if (mcIsEqualIID(riid, IID_IMiaVertexBuffer9))
    {
        AddRef();
        *ppvObject = dynamic_cast<IMiaVertexBuffer9*>(this);
    }

    return MC_OK;
}
