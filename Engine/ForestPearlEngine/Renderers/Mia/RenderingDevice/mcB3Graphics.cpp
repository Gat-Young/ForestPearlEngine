#include "../Mia.h"

#include "../Define/mcB3Class.h"
#include "../VertexBuffer/mcB3VertexBuffer.h"
#include "mcB3Graphics.h"


#define CHECKSTATE( state, val ) (m_RState[(state)] == (val))

#define m_pVB				m_input.pVB
#define m_FVF				m_input.FVF
#define m_Stride			m_input.Stride
#define m_StartVertex		m_input.StartVertex
#define m_TopologyVtxSize	m_input.TopologyVtxSize
#define m_PrimitiveType		m_input.PrimitiveType
#define m_PrimitiveCount	m_input.PrimitiveCount
//#define m_PrimVtxCnt	m_input.PrimTypeVtxSize

//구형 호환 (v1.2 이전)
#define m_PrimCnt			m_stm.CurrPrimDrawCnt
#define m_VtxNum			m_stm.CurrVtxIndex
#define m_FaceNum			m_stm.CurrPrimIndex

#define m_CurrPrimDrawCnt	m_stm.CurrPrimDrawCnt
#define m_CurrVtxIndex		m_stm.CurrVtxIndex
#define m_CurrPrimIndex		m_stm.CurrPrimIndex

#define m_RState			m_pso.RState

#define m_hWnd				m_om.hWnd
#define m_PresentParam		m_om.PresentParam
#define m_hBmpRT			m_om.hBmpRT
#define m_hRT				m_om.hRT
#define m_hSurfaceRT		m_om.hRT


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Mia SWR Engine Core  :  SW 렌더링 엔진 기반 클래스
//
B3MiaCore::B3MiaCore(void)
{

}

B3MiaCore::~B3MiaCore(void)
{

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// B3MiaGraphcisEngine9 : DX9 대응 렌더링 / 그래픽스 엔진 클래스
//
B3MiaGraphicsEngine9::B3MiaGraphicsEngine9(void)
{
	Reset();
}
B3MiaGraphicsEngine9::~B3MiaGraphicsEngine9(void)
{
	Release();
}

/////////////////////////////////////////////////////////////
//
// 외부 메소드
//
int B3MiaGraphicsEngine9::PreDraw(B3MPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	m_input.PrimitiveType = PrimitiveType;
	m_input.PrimitiveCount = PrimitiveCount;
	m_input.StartVertex = StartVertex;

	switch (m_input.PrimitiveType)
	{
		case B3MPT_LINELIST: m_input.TopologyVtxSize = 2;
			break;

		default:
		case B3MPT_TRIANGLELIST: m_input.TopologyVtxSize = 3;
			break;
	}

	return MC_OK;
}

int B3MiaGraphicsEngine9::Draw()
{
	if (MC_INVALIED(m_pVB[0])) return MC_FAIL;
	if (MC_INVALIED(m_Stride)) return MC_FAIL;

	B3MXVECTOR3 vPos[3];
	B3MXVECTOR3 oPos3[3];
	B3MXVECTOR4 oPos4[3];
	B3MXCOLOR	oColor[3];

	m_CurrVtxIndex = m_StartVertex;
	m_CurrPrimIndex = -1;
	m_CurrPrimDrawCnt = 0;

	int res = MC_OK;

	while (1)
	{
		res = _VertexStreaming(vPos, oColor);
		if (MC_FAILED(res))
		{
			break;
		}

		_VertexPipeline(vPos, oPos4, oColor);

		res = _GeometryPipeline(oPos4, oPos3, oColor);
		if (MC_FAILED(res))
		{
		}
		else
		{
			_PixelPipeline(oPos3, oColor);
		}

		{
			m_CurrVtxIndex += m_TopologyVtxSize;
			m_CurrPrimIndex = m_CurrVtxIndex / m_TopologyVtxSize;

			if (++m_CurrPrimDrawCnt >= m_PrimitiveCount)
				break;

			ZeroMemory(vPos, sizeof(B3MXVECTOR3) * 3);
			ZeroMemory(oPos3, sizeof(B3MXVECTOR3) * 3);
			ZeroMemory(oPos4, sizeof(B3MXVECTOR4) * 3);
			ZeroMemory(oColor, sizeof(B3MXCOLOR) * 3);
		}
	}

	return MC_OK;
}

int B3MiaGraphicsEngine9::PostDraw()
{
	return MC_OK;
}

////////////////////////////////
// 렌더링 / 그래픽스 엔진 리셋
int B3MiaGraphicsEngine9::Reset()
{
	m_pDev = NULL;
	ZeroMemory(&m_input, sizeof(INPUT_STREAM));
	ZeroMemory(&m_stm, sizeof(STREAM_DBG));
	ZeroMemory(&m_pso, sizeof(PIPELINE_STATE));
	ZeroMemory(&m_om, sizeof(OUTPUT_MERGE));

	_InitRenderState();
	return MC_OK;
}

////////////////////////////////
// 그래픽스 엔진 해제, 내부 객체 제거
int B3MiaGraphicsEngine9::Release(void)
{
	_RenderTargetRelease();

	_TempBufferRelease();
	return MC_OK;
}


/////////////////////////////////////////////////////////////
//
// 내부 메소드
//
////////////////////////////////
// 백버퍼용 렌더타겟 생성
int  B3MiaGraphicsEngine9::_RenderTargetCreate()
{
	HDC hdc = GetDC(m_hWnd);

	m_hSurfaceRT = CreateCompatibleDC(hdc);
	m_hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, m_PresentParam.Width, m_PresentParam.Height);
	SelectObject(m_hSurfaceRT, m_hBmpRT);

	ReleaseDC(m_hWnd, hdc);
	return MC_OK;
}

////////////////////////////////
// 렌더타겟 제거
void B3MiaGraphicsEngine9::_RenderTargetRelease()
{
	DeleteObject(m_hBmpRT);
	DeleteDC(m_hSurfaceRT);
}

////////////////////////////////
// 렌더타겟 (백버퍼) 의 DC 핸들 획득
HDC B3MiaGraphicsEngine9::_GetDCRT()
{
	return m_hSurfaceRT;
}

////////////////////////////////
// 정점 버퍼를 디바이스에 등록
int B3MiaGraphicsEngine9::_SetVertexBuffer(IMiaVertexBuffer9* pVB, UINT Stride)
{
	if (MC_INVALIED(pVB))
		return MC_FAIL;

	m_pVB[0] = pVB;
	m_Stride = Stride;

	return MC_OK;
}

////////////////////////////////
// 정점 규격 설정
int B3MiaGraphicsEngine9::_SetFVF(DWORD fvf)
{
	m_FVF = fvf;

	return MC_OK;
}

////////////////////////////////
// 각 렌더링 파이프라인에서 생성된 단계별 임시버퍼 제거
void B3MiaGraphicsEngine9::_TempBufferRelease()
{
}

////////////////////////////////
// 장면 그리기 시작
int B3MiaGraphicsEngine9::_BeginScene()
{
	SetBkMode(m_hSurfaceRT, TRANSPARENT);

	HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
	SelectObject(m_hSurfaceRT, hPen);

	return MC_OK;
}

////////////////////////////////
// 장면 그리기 종료
int B3MiaGraphicsEngine9::_EndScene()
{
	return MC_OK;
}

////////////////////////////////
// 렌더타겟 클리어
int B3MiaGraphicsEngine9::_Clear(COLORREF color)
{
	HBRUSH hBrush = CreateSolidBrush(color);
	RECT rc = { 0, 0, (LONG)m_PresentParam.Width,  (LONG)m_PresentParam.Height };
	FillRect(m_hSurfaceRT, &rc, hBrush);
	DeleteObject(hBrush);

	return MC_OK;
}

////////////////////////////////
// 장면 출력
int B3MiaGraphicsEngine9::_Present()
{
	HDC hdc = GetDC(m_hWnd);
	BitBlt(hdc, 0, 0, m_PresentParam.Width, m_PresentParam.Height, m_hSurfaceRT, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hdc);

	return MC_OK;
}

////////////////////////////////
// 정점 버퍼에서 데이터 읽기
int B3MiaGraphicsEngine9::_VertexStreaming(_out_ B3MXVECTOR3 oPosition[3], _out_ B3MXCOLOR oColor[3])
{
	if (MC_INVALIED(m_pVB[0]))
		return MC_FAIL;
	if (MC_INVALIED(m_Stride))
		return MC_FAIL;

	m_pVB[0];

	B3MiaVertexBuffer9* pVB = mcGetClassObject<B3MiaVertexBuffer9>(m_pVB[0]);
	if (MC_INVALIED(pVB))
	{
		return MC_FAIL;
	}

	UINT totVtxSize = pVB->_GetVtxCnt();

	m_CurrPrimIndex = m_CurrVtxIndex / m_TopologyVtxSize;

	if (m_CurrVtxIndex + m_TopologyVtxSize > totVtxSize) return MC_FAIL;

	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		UINT v = m_CurrVtxIndex + i;

		oPosition[i] = pVB->_GetPos3(v);
		oColor[i] = pVB->_GetDiffuse(v);
	}

	return MC_OK;
}

////////////////////////////////
// 정점 파이프 라인 연산
int B3MiaGraphicsEngine9::_VertexPipeline(_in_ B3MXVECTOR3 iPosition[3], _out_ B3MXVECTOR4 oPosition[3], _inout_ B3MXCOLOR oColor[3])
{
	std::vector<VS_OUTPUT> vsouts(m_TopologyVtxSize);

	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		B3MXVECTOR3 pos = iPosition[i];
		B3MXCOLOR	col = oColor[i];

		vsouts[i] = _TnL_VP(pos, col);
	}

	_PrimitiveAssembly(vsouts, oPosition, oColor);
	return MC_OK;
}

////////////////////////////////
// 기하 파이프라인 연산
int B3MiaGraphicsEngine9::_GeometryPipeline(_in_ B3MXVECTOR4 iPosition[3], _out_ B3MXVECTOR3 oPosition[3], _inout_ B3MXCOLOR oColor[3])
{
	int res = MC_OK;

	res = _TnL_GP(iPosition, oPosition, oColor);
	return res;
}

////////////////////////////////
// 픽셀 파이프 라인 연산
int B3MiaGraphicsEngine9::_PixelPipeline(_in_ B3MXVECTOR3 iPosition[3], _in_ B3MXCOLOR oColor[3])
{
	B3MXVECTOR2 pos[3];
	pos[0] = iPosition[0];
	pos[1] = iPosition[1];
	pos[2] = iPosition[2];

	_PixelPipeline1(_기능_생략_);

	_PixelPipeline2(_기능_생략_);

	{
		#define v	pos
		#define c	oColor

		if (m_RState[B3MRS_FILLMODE] == B3MFILL_WIREFRAME)
		{
			_Line(v[0], v[1], c[0], c[1]);
			_Line(v[0], v[2], c[0], c[2]);
			_Line(v[1], v[2], c[1], c[2]);
		}
		else
		{
			_Face(v, c);
		}
		#undef v
		#undef c
	}

	return MC_OK;
}

////////////////////////////////
// 기하 도형 재구성
int B3MiaGraphicsEngine9::_PrimitiveAssembly(_in_ VSOUTS& vsouts, _out_ B3MXVECTOR4 oPosition[3], _out_ B3MXCOLOR oColor[3])
{
	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		oPosition[i] = vsouts[i].pos;
		oColor[i] = vsouts[i].diff;
	}

	return MC_OK;
}

////////////////////////////////
// 픽셀 색상 및 텍스처 혼합
int B3MiaGraphicsEngine9::_PixelPipeline1()
{
	return MC_OK;
}

////////////////////////////////
// 픽셀 출력 테스트 (AB, AT, DST) 및 최종색 출력(RT)
int B3MiaGraphicsEngine9::_PixelPipeline2()
{
	return MC_OK;
}

////////////////////////////////
// 삼각형 컬링 (2D)
bool B3MiaGraphicsEngine9::_FaceCulling2(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2)
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

////////////////////////////////
// 삼각형 컬링 (3D)
bool B3MiaGraphicsEngine9::_FaceCulling3(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2)
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
		if (z < 0)
			bCull = true;
	}
	break;

	case B3MCULL_CCW:
	{
		B3MXVECTOR2 v01 = v1 - v0;
		B3MXVECTOR2 v02 = v2 - v0;
		float z = B3MXVec2CCW(&v01, &v02);
		if (z > 0)
			bCull = true;
	}
	break;
	}

	return bCull;
}

////////////////////////////////
// 뷰포트 멥핑
int B3MiaGraphicsEngine9::_Viewport(B3MXVECTOR3 iPos[3], B3MXVECTOR2 oPos[3])
{
	#define vPos  		iPos[i]
	#define vScreen 	oPos[i]

	float w = (float)m_PresentParam.Width;
	float h = (float)m_PresentParam.Height;

	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		vScreen.x = vPos.x * (w / 2) + (w / 2);
		vScreen.y = -vPos.y * (h / 2) + (h / 2);
	}

	#undef vPos
	#undef vScreen

	return MC_OK;
}

////////////////////////////////
// 정점 처리 파이프라인
VS_OUTPUT B3MiaGraphicsEngine9::_TnL_VP(B3MXVECTOR3 pos, B3MXCOLOR col)
{
	VS_OUTPUT output;

	output.pos = pos;
	output.diff = col;

	return output;
}

////////////////////////////////
// 기하 파이프라인
int B3MiaGraphicsEngine9::_TnL_GP(_in_ B3MXVECTOR4 iPosition4[3], _out_ B3MXVECTOR3 oPosition[3], _inout_ B3MXCOLOR Color[3])
{
	B3MXVECTOR2 vScreen[3];

	if (m_TopologyVtxSize >= 3)
	{
		if (_FaceCulling3(iPosition4[0], iPosition4[1], iPosition4[2]))
		{
			return MC_CULLED;
		}
	}

	B3MXVECTOR3 vPos3[3];
	vPos3[0] = iPosition4[0];
	vPos3[1] = iPosition4[1];
	vPos3[2] = iPosition4[2];

	_Viewport(vPos3, vScreen);

	for (UINT i = 0; i < m_TopologyVtxSize; i++)
	{
		oPosition[i].x = vScreen[i].x;
		oPosition[i].y = vScreen[i].y;
		Color[i] = Color[i];
	}

	return MC_OK;
}

////////////////////////////////
// 픽셀 처리 파이프라인
int B3MiaGraphicsEngine9::_TnL_PP(_in_ B3MXVECTOR2 pos[3], _in_ B3MXCOLOR col[3])
{
	return MC_OK;
}

////////////////////////////////
// 렌더링 상태 초기화
void B3MiaGraphicsEngine9::_InitRenderState()
{
	::ZeroMemory(m_RState, sizeof(DWORD) * B3MRS_MAX_);

	m_RState[B3MRS_FILLMODE] = B3MFILL_SOLID;
	m_RState[B3MRS_CULLMODE] = B3MCULL_CCW;
}

////////////////////////////////
// 렌더링 상태 조절
int B3MiaGraphicsEngine9::_SetRenderState(B3MRENDERSTATETYPE State, DWORD Value)
{
	m_RState[State] = Value;

	return MC_OK;
}

////////////////////////////////
// 렌더링 상태 얻기
int B3MiaGraphicsEngine9::_GetRenderState(B3MRENDERSTATETYPE State, DWORD* pValue)
{
	*pValue = m_RState[State];

	return MC_OK;
}

////////////////////////////////
// 라인 그리기
int B3MiaGraphicsEngine9::_Line(B3MXVECTOR2& v0, B3MXVECTOR2& v1, B3MXCOLOR& col)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, col);
	HPEN hOldPen = (HPEN)SelectObject(m_om.hRT, hPen);

	MoveToEx(m_om.hRT, (int)v0.x, (int)v0.y, NULL);
	LineTo(m_om.hRT, (int)v1.x, (int)v1.y);
	SelectObject(m_om.hRT, hOldPen);
	DeleteObject(hPen);

	return MC_OK;
}

////////////////////////////////
//
int B3MiaGraphicsEngine9::_Line(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXCOLOR sc0, B3MXCOLOR ec0)
{
	B3MXVECTOR2 sp = v0;
	B3MXVECTOR2 ep = v1;
	B3MXCOLOR c0 = sc0;
	B3MXCOLOR c1 = ec0;


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

void B3MiaGraphicsEngine9::_Bresenham(LONG h, LONG w, LONG pW, LONG pH, LONG endW, int AddW, BOOL isWX, B3MXCOLOR Sc, B3MXCOLOR Ec)
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

void B3MiaGraphicsEngine9::_DrawHorizontal(LONG sX, LONG eX, LONG Y, B3MXCOLOR cS, B3MXCOLOR cE)
{
	COLORREF PixelColor;

	for (LONG x = sX; x <= eX; x++)
	{
		MCLerp(&PixelColor, (DWORD)cS, (DWORD)cE, (float)(x - sX) / (eX - sX));
		SetPixel(m_hSurfaceRT, x, Y, PixelColor);
	}
}

void B3MiaGraphicsEngine9::_DrawVertical(LONG sY, LONG eY, LONG X, B3MXCOLOR cS, B3MXCOLOR cE)
{
	COLORREF PixelColor;

	for (LONG y = sY; y <= eY; y++)
	{
		MCLerp(&PixelColor, (DWORD)cS, (DWORD)cE, (float)(y - sY) / (eY - sY));
		SetPixel(m_hSurfaceRT, X, y, PixelColor);
	}
}

////////////////////////////////
//
int B3MiaGraphicsEngine9::_Face(B3MXVECTOR2 pos[3], B3MXCOLOR col[3])
{
	#define v0 pos[0]
	#define v1 pos[1]
	#define v2 pos[2]
	#define c0 col[0]
	#define c1 col[1]
	#define c2 col[2]

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

			float S = (float)(Dot(Vw, Vv) * Dot(Vu, Vv) - Dot(Vw, Vu) * Dot(Vv, Vv)) / (float)(Dot(Vu, Vv) * Dot(Vu, Vv) - Dot(Vu, Vu) * Dot(Vv, Vv));
			float T = (float)(Dot(Vw, Vu) * Dot(Vu, Vv) - Dot(Vw, Vv) * Dot(Vu, Vu)) / (float)(Dot(Vu, Vv) * Dot(Vu, Vv) - Dot(Vu, Vu) * Dot(Vv, Vv));
			float R = 1 - S - T;

			bool IsSInRange = (S >= 0 && S <= 1);
			bool IsTInRange = (T >= 0 && T <= 1);
			bool IsRInRange = (R >= 0 && R <= 1);

			if (!(IsSInRange && IsTInRange && IsRInRange))
			{
				continue;
			}

			COLORREF PixelColor = COLORREF(c0 * S + c1 * T + c2 * R);
			SetPixel(m_hSurfaceRT, X, Y, PixelColor);
		}
	}

	#undef v0
	#undef v1
	#undef v2
	#undef c0
	#undef c1
	#undef c2

	return MC_OK;
}

////////////////////////////////
//
void B3MiaGraphicsEngine9::_HLine(HDC hdc, int x1, int x2, int y, B3MXCOLOR c0, B3MXCOLOR c1)
{
}

////////////////////////////////
//
void B3MiaGraphicsEngine9::_SetPixel(int x, int y, B3MXCOLOR color)
{
	::SetPixel(m_hRT, x, y, color);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// B3MiaGraphcisEngine9의 정의와 구별용 정의 해제
//
#undef m_pGraphics

#undef m_pVB
#undef m_FVF
#undef m_Stride
#undef m_StartVertex
#undef m_TopologyVtxSize
#undef m_PrimitiveType
#undef m_PrimitiveCount

#undef m_PrimCnt
#undef m_VtxNum
#undef m_FaceNum

#undef m_CurrPrimDrawCnt
#undef m_CurrVtxIndex
#undef m_CurrPrimIndex

#undef m_RState

#undef m_hWnd
#undef m_PresentParam
#undef m_hBmpRT
#undef m_hRT
#undef m_hSurfaceRT