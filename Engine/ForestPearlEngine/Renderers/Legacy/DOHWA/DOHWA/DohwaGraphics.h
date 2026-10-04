#pragma once

#include <vector>

/////////////////////////////////////////////////////////////////////////////
//
// 정점 파이프라인(VP, Vertex Pipeline) 출력 구조체.
//
struct VS_OUTPUT
{
	DOHWAXVECTOR4 pos;	//위치 
	COLOR	diff;	//확산광 색상 (Diffuse Color)
};

//! 각 VP 처리 결과 저장 배열.
typedef std::vector<VS_OUTPUT>  VSOUTS;

/////////////////////////////////////////////////////////////////////////////// 
//
// Dohwa SWR Engine Core  :  SW 렌더링 엔진 기반 클래스
//
// Dohwa SWR 엔진 기반 클래스  
//  - 공통 데이터 및 인터페이스 제공
// 
// \code 
// <파생 클래스>
//  - DohwaGraphics
//  - DohwaGraphicsEngine9/10/11/GL/VK    //그래픽스 엔진
//  - DohwaCopyEngine						//복사 엔진
//  - DohwaComputeEngine					//계산 엔진
//  - DohwaCUDAEngine						//쿠다 엔진
//  - DohwaRTXEngine						//RTX 엔진
//  - DohwaDLSSEngine						//DLSS 엔진
// \endcode
// 
// \b 지원 버전
// - Dohwa SWR v1.5.3 이상
// 
/////////////////////////////////////////////////////////////////////////////// 

class DohwaCore
{
	friend class DohwaDevice9;
	friend class DohwaDevice9x;

protected:


public:
	DohwaCore(void);
	virtual ~DohwaCore(void);

	//virtual int Draw() pure;

};

/////////////////////////////////////////////////////////////////////////////// 
//
//	class	DohwaGraphicsEngine9  
//	bstar	Dohwa 그래픽스 엔진 클래스
// 
//	렌더링 파이프라인 운용 및 그래픽스 API 구현 (D3D9 대응)
//
// \par [목적]
//  - 렌더링 장치 Device 와 렌더링/그래픽스 Pipeline 기능 분리.
//  - 신규 렌더링 엔진 개량, 배포 및 재사용성 증대
// 
// \par [운용]
// 모든 렌더링(에 사용되는) 객체의 생성과 운용을 담당합니다. 일부 객체는 외부 참조됩니다.
//  - RTs, DS, VBs, IB, CBs, UAVs
//  - Render States, Pipeline State Objects (PSOs)
//  - Lights, Materials,Textures, Samplers
//  - Shaders
//  - Transform Matrices, Viewports   
//  공급된 데이터를 기반으로 그래픽스 연산 - 파이프라인을  진행하며 연산 결과는 
//  (외부 지정) 렌더타겟에 출력됩니다. 더하여 스테이지 별 디버깅 정보 생산 및 관리도 지원합니다.
// 
//  주요 렌더링 작업은 Dohwa GraphicsEngine 에서 처리합니다. (v1.5.3 이상)
//
/////////////////////////////////////////////////////////////////////////////// 
//
class DohwaGraphicsEngine9 : public DohwaCore
{
	friend class DohwaDevice9;
	friend class DohwaDevice9x;


public:

	// 파이프라인 입력 정보 및 객체 운용 : VB, IB, FVF 등...
	// 
	struct INPUT_STREAM
	{
		//IDohwaVertexBuffer9* pVB[16];		//< 렌더링용 등록된 정점 버퍼.<DX9>
		IDohwaVertexBuffer9*	pVB[1];		//< 렌더링용 등록된 정점 버퍼.<Yena>
		DWORD FVF;							//< 렌더링용 등록된 정점 규격
		UINT  Stride;						//< 렌더링용 등록된 정점 1마디의 크기


		//그리기 옵션.(DrawPrimitive 호출시 입력됨)
		DOHWAPRIMITIVETYPE PrimitiveType;		//< 이번에 그려질 기하도형 타입. (디버깅용)
		UINT PrimitiveCount;					//< 이번에 그려질 기하도형 개수. (Line, Face)
		UINT StartVertex;						//< 이번에 그려질 (시작) 정점 번호.
		UINT TopologyVtxSize;					//< 이번에 그려질 기하도형 구성 정점 개수 : Triangle-List = 1 Face, 3 Vertices, Line-List = 1 Line, 2 Vertices
	};


	//! 렌더링 - 스트리밍 실행중 데이터 (디버깅용)
	//! 
	struct STREAM_DBG
	{
		UINT  CurrVtxIndex;					//< 현재 작업중인 정점 번호. 디버깅용.
		//UINT  FaceNum;					//< 현재 작업중인 페이스 번호. 디버깅용.
		UINT  CurrPrimIndex;				//< 현재 작업중인 기하도형 번호. 디버깅용.
		UINT  CurrPrimDrawCnt;				//< 현재 작업중인 기하도형 개수. 
	};

	// 변환용 데이터 운용
	//
	struct TRANSFORM
	{
		DOHWAXMATRIX mTFM[DOHWATS_MAX_];		//!< 변환 행렬들, 배열  ★
		//DOHWAXMATRIX mInvProj;				//!< 원근 보정용 행렬.
	};

	// 파이프라인 상태 객체 운용
	// 
	struct PIPELINE_STATE
	{
		DWORD  RState[DOHWARS_MAX_];			//!< 렌더링 상태 값 
	};


	// 출력/혼합 단계 정보 및 객체 운용 : 렌더타겟, 뷰포트 등..
	//
	struct OUTPUT_MERGE
	{
		HWND hWnd;									//< 출력 윈도 핸들
		DOHWAPRESENT_PARAMETERS	PresentParam;		//< 출력 정보 (해상도, 포멧 등)

		HBITMAP  hBmpRT;							//< 렌더타겟(Back-Buffer) 구성용 핸들.		
		HDC		 hRT;								//< 렌더타겟 DC 핸들
		#define hSurfaceRT hRT						//< 렌더타겟 DC 핸들.(구형호환성 유지용)		
	};



protected:

	DohwaDevice9* m_pDev;			//< 렌더링 장치 (외부 참조)

	//그래픽 엔진 - 파이프라인 별 운용 데이터
	INPUT_STREAM	m_input;		//< 파이프라인 입력 정보 및 객체 운용 : VB, IB, FVF 등...
	STREAM_DBG		m_stm;			//< 렌더링 - 스트리밍 실행중 데이터 (디버깅용)
	TRANSFORM		m_tms;			//< 변환용 데이터 운용
	PIPELINE_STATE	m_pso;			//< 파이프라인 상태 객체 운용
	OUTPUT_MERGE	m_om;			//< 출력/혼합 단계 정보 및 객체 운용 : 렌더타겟, 뷰포트 등..	

	//SHADER_STAGE	m_ss;

	//디버깅 정보 : BegineScene 부터 EndScene 까지 사용된 정보 누계. 이후 리셋됨. 
	//B3YDBGINFO	m_DbgInfo;


protected:
	//---------------------------
	// 내부 메소드들 : '밑줄 _ '접두어 표시.
	//---------------------------
	//...

	//---------------------------
	// 자원 - 버퍼 운용
	//---------------------------
	int  RenderTargetCreate();
	void RenderTargetRelease();
	HDC	 GetDCRT();

	int SetVertexBuffer(IDohwaVertexBuffer9* pVB, UINT Stride);
	int SetFVF(DWORD fvf);

	void TempBufferRelease();


	//---------------------------
	// 장면 관리
	//---------------------------
	virtual int BeginScene();
	virtual int EndScene();
	virtual int Clear(COLORREF col);
	virtual int Present();



	//---------------------------
	// 파이프라인 스테이지
	//---------------------------
	virtual int VertexStreaming(_out_ DOHWAXVECTOR3 oPos[3], _out_ DOHWAXCOLOR oColor[3]);
	//virtual int VertexAssembly	();
	virtual int VertexPipeline(_in_ DOHWAXVECTOR3 iPos[3], _out_ DOHWAXVECTOR4 oPos[3], _inout_ DOHWAXCOLOR oColor[3]);
	virtual int GeometryPipeline(_in_ DOHWAXVECTOR4 iPos[3], _out_ DOHWAXVECTOR3 oPos[3], _inout_ DOHWAXCOLOR oColor[3]);
	virtual int PixelPipeline(_in_ DOHWAXVECTOR3 oPos[3], _in_  DOHWAXCOLOR oColor[3]);

	virtual int PrimitiveAssembly(_in_ VSOUTS& vsouts, _out_ DOHWAXVECTOR4 oPos[3], _out_ DOHWAXCOLOR oColor[3]);
	//virtual int Rasterization		(_inout_ DOHWAXVECTOR3 oPos[3], _inout_ DOHWAXCOLOR oColor[3]);
	virtual int PixelPipeline1();
	virtual int PixelPipeline2();


	virtual bool FaceCulling2(DOHWAXVECTOR2 v0, DOHWAXVECTOR2 v1, DOHWAXVECTOR2 v2);
	virtual bool FaceCulling3(DOHWAXVECTOR2 v0, DOHWAXVECTOR2 v1, DOHWAXVECTOR2 v2);
	virtual int  Viewport(DOHWAXVECTOR3 iPos[3], DOHWAXVECTOR2 oPos[3]);


	//---------------------------
	// 고정함수 스테이지
	//---------------------------
	//virtual VS_OUTPUT _TnL_VP(_inout_ DOHWAXVECTOR3 oPos[3], _inout_ DOHWAXCOLOR oColor0[3], _inout_ DOHWAXVECTOR3 oNormal[3], _inout_ DOHWAXVECTOR2 oTexCoord[3]);
	virtual VS_OUTPUT TnL_VP(DOHWAXVECTOR3 pos, DOHWAXCOLOR col);
	virtual int		  TnL_GP(_in_ DOHWAXVECTOR4 pos[3], _out_ DOHWAXVECTOR3 oPos[3], _inout_ DOHWAXCOLOR col[3]);
	virtual int		  TnL_PP(_in_ DOHWAXVECTOR2 pos[3], _in_ DOHWAXCOLOR col[3]);


	//---------------------------
	// 셰이더 스테이지
	//---------------------------
	//virtual VS_OUTPUT _VS_Main (DOHWAXVECTOR3 pos, DOHWAXCOLOR col);
	//virtual int		_GS_Main ();
	//virtual int		_PS_Main (DOHWAXCOLOR color, _out_ DOHWAXCOLOR& oColor);



	//---------------------------
	// 파이프라인 상태 조절
	//---------------------------
	virtual void InitRenderState();
	virtual int  SetRenderState(DOHWARENDERSTATETYPE State, DWORD Value);
	virtual int  GetRenderState(DOHWARENDERSTATETYPE State, DWORD* pValue);

	//--------------------------------
	// 변환 행렬 설정
	//--------------------------------
	virtual int SetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* mTM);
	virtual int GetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* mTM);

	//---------------------------
	// 그래픽스 API
	//---------------------------
	//그리기 함수.: 클래스 DOHWAXVECTOR 타입으로 갱신.
	virtual int	 Line(DOHWAXVECTOR2& v0, DOHWAXVECTOR2& v1, DOHWAXCOLOR& col);
	virtual int  Line(DOHWAXVECTOR2 v0, DOHWAXVECTOR2 v1, DOHWAXCOLOR sc0, DOHWAXCOLOR ec0);
	virtual int  Face(DOHWAXVECTOR2 pos[3], DOHWAXCOLOR  col[3]);
	virtual void HLine(HDC hdc, int x1, int x2, int y, DOHWAXCOLOR c0, DOHWAXCOLOR c1);

	// 단순 색상 출력. 대부분의 기능은 Rasterizer 로 이전.
	virtual void SetPixel(int x, int y, DOHWAXCOLOR color);

	//레스터 라이저
	// Rasterizer : 픽셀 혼합 및 출력 관련 중요 스테이지.
	//            : 기존의 _SetPixel 의 기능 확장. Rasterizer Stage (DX10/11) 대응. 
	//void _Rasterizer(HDC hDC, int x, int y, DOHWAXCOLOR color0, DOHWAXCOLOR color1, float z, DOHWAXVECTOR2 tex);


	//---------------------------
	// 디버깅 정보 획득
	//---------------------------
	//B3YDBGINFO* _GetDbgInfo();


public:
	DohwaGraphicsEngine9(void);
	virtual ~DohwaGraphicsEngine9(void);

	virtual int PreDraw(DOHWAPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount);
	virtual int Draw();
	virtual int PostDraw();

	virtual int Reset();		//< 렌더링 엔진 리셋.
	virtual int Release();		//< 렌더링 엔진 자원 해제.

	virtual void SetDev(DohwaDevice9* pDev) { m_pDev = pDev; }

};
