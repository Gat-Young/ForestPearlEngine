#pragma once
#include "../Define/mcDefine.h"

/////////////////////////////////////////////////////////////
//
// Mia 그래픽스 상수
//
#define _omitted_
#define _기능_생략_

/////////////////////////////////////////////////////////////
//
// 정점 파이프라인(VP, Vertex Pipeline) 출력 구조체
//
struct VS_OUTPUT
{
	VECTOR4 pos;	//위치
	COLOR	diff;	    //확산광 색상 (Diffuse Color)
};

////////////////////////////////
// 각 VP 처리 결과 저장 배열
typedef std::vector<VS_OUTPUT>  VSOUTS;



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// Mia SWR Engine Core  :  SW 렌더링 엔진 기반 클래스
//
class B3MiaCore
{
	friend class B3MiaDevice9;
	friend class B3MiaDevice9x;
protected:
public:
	B3MiaCore(void);
	virtual ~B3MiaCore(void);

};
typedef B3MiaCore* LPB3MIACORE;
typedef B3MiaCore* LPB3MIAENGINECORE;



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
//
// B3MiaGraphicsEngine9 :  Mia 그래픽스 엔진 클래스
// B3MiaVertexBuffer9에서 가져옴
//
class B3MiaGraphicsEngine9 : public B3MiaCore
{
	friend class B3MiaDevice9;
	friend class B3MiaDevice9x;

public:
	B3MiaGraphicsEngine9(void);
	virtual ~B3MiaGraphicsEngine9(void);

public:
	struct INPUT_STREAM
	{
		LPMIAVERTEXBUFFER9	pVB[1];
		DWORD FVF;
		UINT  Stride;

		B3MPRIMITIVETYPE PrimitiveType;
		UINT PrimitiveCount;
		UINT StartVertex;
		UINT TopologyVtxSize;
	};

	struct STREAM_DBG
	{
		UINT  CurrVtxIndex;
		UINT  CurrPrimIndex;
		UINT  CurrPrimDrawCnt;
	};

	struct PIPELINE_STATE
	{
		DWORD  RState[B3MRS_MAX_];
	};

	struct OUTPUT_MERGE
	{
		HWND hWnd;
		MCPRESENT_PARAM	PresentParam;

		HBITMAP  hBmpRT;
		HDC		 hRT;
		#define hSurfaceRT hRT
	};

protected:
	B3MiaDevice9* m_pDev;
	INPUT_STREAM	m_input;
	STREAM_DBG		m_stm;
	PIPELINE_STATE	m_pso;
	OUTPUT_MERGE	m_om;

public:
	virtual int PreDraw(B3MPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	virtual int Draw();
	virtual int PostDraw();

	virtual int Reset();
	virtual int Release();
	virtual void SetDev(B3MiaDevice9* pDev) { m_pDev = pDev; }

/////////////////////////////////////////////////////////////
//
// 내부 메소드
//
protected:
	////////////////////////////////
	// 자원 - 버퍼 운용
	int  _RenderTargetCreate();
	void _RenderTargetRelease();
	HDC	 _GetDCRT();

	int _SetVertexBuffer(IMiaVertexBuffer9* pVB, UINT Stride);
	int _SetFVF(DWORD fvf);
	void _TempBufferRelease();

	////////////////////////////////
	// 장면 관리
	virtual int _BeginScene();
	virtual int _EndScene();
	virtual int _Clear(COLORREF color);
	virtual int _Present();

	////////////////////////////////
	// 파이프라인 스테이지
	virtual int _VertexStreaming(_out_ B3MXVECTOR3 oPosition[3], _out_ B3MXCOLOR oColor[3]);
	virtual int _VertexPipeline(_in_ B3MXVECTOR3 iPosition[3], _out_ B3MXVECTOR4 oPosition[3], _inout_ B3MXCOLOR oColor[3]);
	virtual int _GeometryPipeline(_in_ B3MXVECTOR4 iPosition[3], _out_ B3MXVECTOR3 oPosition[3], _inout_ B3MXCOLOR oColor[3]);
	virtual int _PixelPipeline(_in_ B3MXVECTOR3 oPosition[3], _in_  B3MXCOLOR oColor[3]);
	virtual int _PrimitiveAssembly(_in_ VSOUTS& vsouts, _out_ B3MXVECTOR4 oPosition[3], _out_ B3MXCOLOR oColor[3]);
	virtual int _PixelPipeline1();
	virtual int _PixelPipeline2();

	virtual bool _FaceCulling2(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2);
	virtual bool _FaceCulling3(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXVECTOR2 v2);
	virtual int  _Viewport(B3MXVECTOR3 iPosition[3], B3MXVECTOR2 oPosition[3]);

	////////////////////////////////
	// 고정 함수 스테이지
	virtual VS_OUTPUT _TnL_VP(B3MXVECTOR3 pos, B3MXCOLOR col);
	virtual int		  _TnL_GP(_in_ B3MXVECTOR4 iPosition4[3], _out_ B3MXVECTOR3 oPosition[3], _inout_ B3MXCOLOR Color[3]);
	virtual int		  _TnL_PP(_in_ B3MXVECTOR2 pos[3], _in_ B3MXCOLOR col[3]);

	////////////////////////////////
	// 셰이더 스테이지
	//virtual VS_OUTPUT _VS_Main (B3MXVECTOR3 pos, B3MXCOLOR col);
	//virtual int		_GS_Main ();
	//virtual int		_PS_Main (B3MXCOLOR color, _out_ B3MXCOLOR& oColor);

	////////////////////////////////
	// 파이프라인 상태 조절
	virtual void _InitRenderState();
	virtual int  _SetRenderState(B3MRENDERSTATETYPE State, DWORD Value);
	virtual int  _GetRenderState(B3MRENDERSTATETYPE State, DWORD* pValue);

	////////////////////////////////
	// 그래픽스 API
	virtual int	 _Line(B3MXVECTOR2& v0, B3MXVECTOR2& v1, B3MXCOLOR& col);
	virtual int  _Line(B3MXVECTOR2 v0, B3MXVECTOR2 v1, B3MXCOLOR sc0, B3MXCOLOR ec0);
	virtual void _Bresenham(LONG h, LONG w, LONG pW, LONG pH, LONG endW, int AddW, BOOL isWX, B3MXCOLOR Sc, B3MXCOLOR Ec);
	virtual void _DrawHorizontal(LONG sX, LONG eX, LONG Y, B3MXCOLOR cS, B3MXCOLOR cE);
	virtual void _DrawVertical(LONG sY, LONG eY, LONG X, B3MXCOLOR cS, B3MXCOLOR cE);
	virtual int  _Face(B3MXVECTOR2 pos[3], B3MXCOLOR  col[3]);
	virtual void _HLine(HDC hdc, int x1, int x2, int y, B3MXCOLOR c0, B3MXCOLOR c1);

	virtual void _SetPixel(int x, int y, B3MXCOLOR color);
};

typedef B3MiaGraphicsEngine9		B3MiaGraphics;
typedef B3MiaGraphicsEngine9* LPB3MIAGRAPHICSENGINE9;
typedef LPB3MIAGRAPHICSENGINE9		LPGRAPHICS;