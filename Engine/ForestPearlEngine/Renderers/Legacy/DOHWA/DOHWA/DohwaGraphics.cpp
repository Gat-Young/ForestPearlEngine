#include "Dohwa.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////////////
//
// Dohwa SWR 구현 클래스 선언 : 외부 노출 방지용.
//
#include "DohwaClass.h"
#include "DohwaVertexBuffer.h"
#include "DohwaGraphics.h"

#define CHECKSTATE( state, val ) (m_RState[(state)] == (val))	

/////////////////////////////////////////////////////////////////////////////// 
//
// Dohwa SWR Engine Core  :  SW 렌더링 엔진 기반 클래스
//
/////////////////////////////////////////////////////////////////////////////// 

DohwaCore::DohwaCore(void)
{
	//... 
}

DohwaCore::~DohwaCore(void)
{
	//... 
}

/////////////////////////////////////////////////////////////////////////////// 
//
// DohwaGraphcisEngine9 : DX9 대응 렌더링 / 그래픽스 엔진 클래스, 
//
/////////////////////////////////////////////////////////////////////////////// 


DohwaGraphicsEngine9::DohwaGraphicsEngine9(void)
{
	Reset();
}


DohwaGraphicsEngine9::~DohwaGraphicsEngine9(void)
{
	Release();
}

//////////////////////////////////////////////////////////////////////////////
//
// 렌더링 / 그래픽스 엔진 리셋.
//
//////////////////////////////////////////////////////////////////////////////

int DohwaGraphicsEngine9::Reset()
{
	m_pDev = NULL;

	ZeroMemory(&m_input, sizeof(INPUT_STREAM));
	ZeroMemory(&m_stm, sizeof(STREAM_DBG));
	ZeroMemory(&m_tms, sizeof(TRANSFORM));
	ZeroMemory(&m_pso, sizeof(PIPELINE_STATE));
	ZeroMemory(&m_om, sizeof(OUTPUT_MERGE));

	//디버깅 정보 초기화.
	///m_DbgInfo.ClearAll();

	//변환 행렬 클리어.
	for (int i = 0; i < DOHWATS_MAX_; i++)
	{
		DOHWAXMatrixIdentity(&m_tms.mTFM[i]); 	//ver.YN ★
	}


	//렌더상태 초기화.
	InitRenderState();


	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 그래픽스 엔진 해제, 내부 객체 제거
//
int DohwaGraphicsEngine9::Release(void)
{
	//Back-Buffer : Render Target 제거. 
	RenderTargetRelease();


	//정점 버퍼 자체를 제거하는 것은 사용자가 책임을 집니다.
	//...SafeRelease(m_input.pVB);	

	//각 렌더링 단계별 임시버퍼 제거.
	TempBufferRelease();

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 각 렌더링 파이프라인에서 생성된 단계별 임시버퍼 제거. 
//
void DohwaGraphicsEngine9::TempBufferRelease(void)
{
	//... 
}

///////////////////////////////////////////////////////////////////////////////
//
// 백버퍼용 렌더타겟을 생성.
//
int DohwaGraphicsEngine9::RenderTargetCreate()
{
	HDC hdc = GetDC(m_om.hWnd);

	m_om.hRT = CreateCompatibleDC(hdc);
	m_om.hBmpRT = (HBITMAP)CreateCompatibleBitmap(hdc, m_om.PresentParam.Width, m_om.PresentParam.Height);
	SelectObject(m_om.hRT, m_om.hBmpRT);

	ReleaseDC(m_om.hWnd, hdc);

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
// 
// 렌더타겟 제거.
//
void DohwaGraphicsEngine9::RenderTargetRelease()
{
	DeleteObject(m_om.hBmpRT);
	DeleteDC(m_om.hRT);
}

////////////////////////////////////////////////////////////////////////////////
//
// 렌더타겟 (백버퍼) 의 DC 핸들 획득
// 
// \return		성공시 DC 핸들 리턴.
//
HDC DohwaGraphicsEngine9::GetDCRT()
{
	return m_om.hRT;
}

///////////////////////////////////////////////////////////////////////////////
//
// 장면 그리기 시작
// : 렌더링에 필요한 (디바이스의) 선위 작업을 수행.
//
int DohwaGraphicsEngine9::BeginScene()
{
	//폰트 배경색 기본값 
	//SetBkColor(m_hSurfaceRT, m_BkColor);
	SetBkMode(m_om.hRT, TRANSPARENT);

	//기본 펜 색상 .
	HPEN  hPen = (HPEN)GetStockObject(WHITE_PEN);
	SelectObject(m_om.hRT, hPen);

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 장면 그리기 종료
//
//: 렌더링 종료에 필요한 (디바이스의) 후위 작업을 수행.
//
int DohwaGraphicsEngine9::EndScene()
{
	//...

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 렌더타겟 클리어.
//
// 렌더링 파이프라인에 등록(Binding)된 이미지 버퍼(서피스, Surface)를 지웁니다.  
// 하나 이상의 렌더타겟과 깊이버퍼, 스텐실버퍼를 지울 수 있습니다.
//
// col		RT 을 지울 지정색.
// 성공시 OK, 실패시 FALSE
//
int DohwaGraphicsEngine9::Clear(COLORREF col)
{
	HBRUSH hBrush = CreateSolidBrush(col);
	RECT rc = { 0, 0, (LONG)m_om.PresentParam.Width,  (LONG)m_om.PresentParam.Height };
	FillRect(m_om.hRT, &rc, hBrush);
	DeleteObject(hBrush);

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 장면 출력
// 
// RT 의 내용(렌더링된 장면)을 Front Buffer 에 출력합니다. "Flipping", "Swapping"
// 
int DohwaGraphicsEngine9::Present()
{
	HDC hdc = GetDC(m_om.hWnd);
	BitBlt(hdc, 0, 0, m_om.PresentParam.Width, m_om.PresentParam.Height, m_om.hRT, 0, 0, SRCCOPY);
	ReleaseDC(m_om.hWnd, hdc);

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 정점 버퍼 등록.
// 
// 정점 버퍼를 디바이스에 등록합니다. 
// 
// <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
// 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.  
// <Dohwa> 1개의 정점 버퍼만 사용합니다.
//
// pVB		렌더링 할 정점 버퍼 포인터.
// Stride	렌더링 할 버퍼의 1마디(정점 데이터) 크기
//
int DohwaGraphicsEngine9::SetVertexBuffer(IDohwaVertexBuffer9* pVB, UINT Stride)
{
	if (DOHWA_INVALIED(pVB)) return DOHWA_FAIL;


	// <DX> 복수의 정점 버퍼로 동시 '기하 스트리밍'을 지원합니다. (DX9, 최대 16)
	// 또는 지정 VB 내의 데이터중 부분 렌더링(OffsetBytes)도 가능합니다.
	// m_pVB[StreamNumber] = pVB;
	// 
	// <Dohwa> 1개의 정점 버퍼만 사용합니다.
	m_input.pVB[0] = pVB;
	m_input.Stride = Stride;

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 정점 규격 설정.
// 
// 정점 규격을 설정합니다. 
// 렌더링시 엔진에 등록된 정점과 동일한 규격을 명시해야 합니다.
// 그렇지 않다면 정상적인 렌더링 결과를 기대할 수 없습니다.
// 
//  fvf	정점규격 (FVF)
//
int DohwaGraphicsEngine9::SetFVF(DWORD fvf)
{
	m_input.FVF = fvf;

	return DOHWA_OK;
}


///////////////////////////////////////////////////////////////////////////////
//
// Yena Graphics Engine : Pipeline API
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// 정점 버퍼에서 데이터 읽기 "Vertex Streaming"
// 
// "Triangle-List" 렌더링 모드로 동작시  VB 에서 1개의 페이스(3개의 정점)씩 처리
//  - Triangle-List = 1 Face, 3 Vertices
//  - Line-List = 1 Line, 2 Vertices
//
// [out]	oPosition	(처리할) 정점 위치
// [out]	oColor0		(처리할) 정점 색상 (Diffuse Color)
//
int DohwaGraphicsEngine9::VertexStreaming(
	_out_ DOHWAXVECTOR3 oPosition[3],
	_out_ DOHWAXCOLOR   oColor[3]
)
{

	if (DOHWA_INVALIED(m_input.pVB[0])) return DOHWA_FAIL;
	if (DOHWA_INVALIED(m_input.Stride)) return DOHWA_FAIL;

	// 정점 버퍼 운용 객체 접근 : VB 0번 고정
	DohwaVertexBuffer9* pVB = DhGetClassObject<DohwaVertexBuffer9>(m_input.pVB[0]);
	if (DOHWA_INVALIED(pVB))
	{
		//에러 처리...
		return DOHWA_FAIL;
	}


	// 현재 디바이스에 등록된 VB 의 기본정보를 계산. 
	//
	UINT totVtxSize = pVB->GetVtxCnt();				//(전체) 정점 개수. 
	//m_CurrVtxNum = -1;							//현재 처리중인 정점번호.디버깅용
	//m_CurrPrimNum = -1;

	//(현재 작업할) 기하도형 색인(번호) 계산
	m_stm.CurrPrimDrawCnt = m_stm.CurrVtxIndex / m_input.TopologyVtxSize;

	//정점 버퍼 범위 검사, 범위를 벗어나면 작업 종료..
	if (m_stm.CurrVtxIndex + m_input.TopologyVtxSize > totVtxSize) return DOHWA_FAIL;


	// 정점 버퍼에서 기하 도형별 정점 정보 읽기. "Vertex Streaming"
	// "Triangle-List" 렌더링 모드로 동작시  VB 에서 1개의 페이스(3개의 정점)씩 처리합니다.
	//  - Triangle-List = 1 Face, 3 Vertices
	//  - Line-List = 1 Line, 2 Vertices
	for (UINT i = 0; i < m_input.TopologyVtxSize; i++)		//지정 정점부터, 읽기.
	{
		UINT v = m_stm.CurrVtxIndex + i;					//읽어들일 정점 번호
		//m_dbg.CurrVtxIndex = v;						//읽어들일 정점 번호.디버깅용

		//정점 데이터 얻기.
		oPosition[i] = pVB->GetPos3(v);				//정점 좌표 (3성분)
		oColor[i] = pVB->GetDiffuse(v);				//정점 색상 (4성분) 원본 정점버퍼에 DWORD 형으로 입력되더라도, XCOLOR 타입으로 처리합니다.(편의성 증대)
	}

	//3. 외부로 정점 리턴
	//...

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 정점 파이프 라인 연산을 수행. 
// 
// 각 Vertex 별 연산 Operation - 정점별 변환, 조명, 안개 등의 연산을 처리합니다.  
//   - 정점 변환 (World Transform)  
//   - 정점 블랜딩 (Vertex Blending)  
//   - 정점 변환 (View Transform)  
//   - 정점 안개 (Vertex Fog)  
//   - 정점 조명 (Vertex Lighting)  
//   - 정점 변환 (Projection Transform)  
// 출력결과는 기하 파이프라인 (Geometry Pipeline, GP) 으로 넘어감니다.  
// 
// 정점 셰이더(VS, Vertex Shader)를 통한 사용자 연산을 처리할 수 있습니다.  
// <Dohwa> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.  
//        더하여  VS 및 HLSL 형식의 개발환경도 함께 제작합니다.
// 
// iPosition	(처리할) 현재 정점 좌표, 3성분 
// [out]		oPosition	(처리된) 현재 정점 좌표, 4성분 ★
// [in, out]	oColor		(처리할) 현재 정점 색상 (Diffuse Color)
//
int DohwaGraphicsEngine9::VertexPipeline(
	_in_	DOHWAXVECTOR3 iPosition[3],
	_out_   DOHWAXVECTOR4 oPosition[3],
	_inout_ DOHWAXCOLOR	oColor[3]
)
{

	//각 VP 처리 결과 저장 배열.
	std::vector<VS_OUTPUT> vsouts(m_input.TopologyVtxSize);


	//현재 렌더링 기하 형식 (Primitive Topology) 맞추어 정점별 연산을 수행...
	//...
	for (UINT i = 0; i < m_input.TopologyVtxSize; i++)
	{
		DOHWAXVECTOR3 pos = iPosition[i];
		DOHWAXCOLOR	col = oColor[i];


		//정점 셰이더 스테이지 (Vertex Shader Stage)
		//...
		//vsouts[i] = _VS_Main(pos, col);


		//정점 고정함수 파이프라인 (Fixed Function Pipeline)
		vsouts[i] = TnL_VP(pos, col);
	}


	//전체 VP 연산 결과 조합, 다음 파이프라인으로 출력...
	//...
	PrimitiveAssembly(vsouts, oPosition, oColor);


	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
//  기하 파이프라인 연산을 수행.
// 
// 각 Face 별 연산 Operation 을 처리합니다.  
//   - 삼각형 컬링 (Face Culling)  
//   - 사용자 평면 클립핑 (User Plane Clipping)   
//   - 시야 공간 클립핑 (Viewing Frustum Clipping)   
//   - 동차 나누기 (Homogeneous Divide)  
//   - 뷰포팅 (View Port) (3D->2D)  
// 
// 출력결과는 픽셀 파이프라인 (Pixel Pipeline, PP) 으로 넘어감니다.  
// 
// <DX9> 고정함수 파이프라인만 지원. DX10 이상, 기하 셰이더 (Geometry Shader) 지원  
// <Dohwa> 고정함수 파이프라인만 지원.  
// 
//  iPosition				(입력) 정점 좌표, 4성분 ★
//  [out]		oPosition	(출력) 정점 좌표, 3성분 ★
//  [in, out]	oColor		(출력) 정점색 (Diffuse Color)
//
int DohwaGraphicsEngine9::GeometryPipeline(
	_in_    DOHWAXVECTOR4 iPosition[3],
	_out_   DOHWAXVECTOR3 oPosition[3],
	_inout_ DOHWAXCOLOR	oColor[3]
)
{
	int res = DOHWA_OK;

	//기하 셰이더 스테이지 (Geometry Shader Stage)
	//<DX10> 이상 지원, <Dohwa> 미지원
	//...
	//_GS_Main(oPosition, oColor, oNormal, oTexCoord);


	//기하 고정함수 파이프라인 (Fixed Function Pipeline)
	res = TnL_GP(iPosition, oPosition, oColor);


	return res;
}

////////////////////////////////////////////////////////////////////////////////
//
// 픽셀 파이프 라인 연산을 수행.
//
// 픽셀 Pixel 별 연산 Operation 을 처리하는 중요 스테이지(함수) 중 하나입니다.  
// 다음의 연산이 순차적으로 처리됩니다.  
//
//   * Pixel Pipeline #1
//     - 텍스처 혼합 (Texture Stage)  
//     - 조명 혼합 (Specular Light Blending)  
//     - 픽셀 안개 혼합 (Pixel Fog Blending)  
//   * Pixel Pipeline #2
//     - 안개 혼합 (Fog Blending)  
//		- 가위 테스트 (Scissor Test)   
//		- 알파 테스트 (Alpha Test)  
//	- 깊이/스텐실 테스트 (Depth/Stencil Test)  
//		- 알파 블렌딩 (Alpha Blending)  
//		- 디더링 (Dithering)  
//		- 렌더타겟 쓰기 마스크 (Rendering Channel Mask)  
//		- 렌더타겟에 색상 기록 (Present, Write to Render Target)  
// 
// 색상 출력은 필수이며 연산 결과는 렌더타겟(RenderTarget, RT) 에 기록됩니다.   
// 
// <DX> 셰이더는 레거시 고정 함수 파이프라인(Fixed Function Pipeline) 의 기능을 사용자의 
// 선택에 따라 조정하거나 변경할 수 있습니다. 또한 픽셀 셰이더(Pixel Shader, PS) 를 통해 
// 여러분만의 기능으로 독자적인 파이프라인 구축이 가능합니다.  
// 픽셀 셰이더(Pixel Shader, PS) 는 GPU 프로그래밍 언어(HLSL) 로 개발되며 사용자 연산을 처리할 수 있습니다.
// 
// <DX9> 고정 함수 파이프라인 및 픽셀 셰이더 지원  
// <Dohwa> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.
//        더하여  PS 및 HLSL 형식의 개발환경도 함께 제작합니다.  
// 
//  pos3	픽셀 좌표, 배열
//  color	픽셀 색상, 배열
//
int DohwaGraphicsEngine9::PixelPipeline(
	_in_ DOHWAXVECTOR3 pos3[3],
	_in_ DOHWAXCOLOR	 color[3]
)
{

	//이전 스테이지에서 2D 변환된 정점이 공급됩니다.★ 
	DOHWAXVECTOR2 pos[3];
	pos[0] = pos3[0];
	pos[1] = pos3[1];
	pos[2] = pos3[2];


	//------------------------------------------
	// 1. 레스터라이저 : 2D 픽셀 정보 산출 (보간)
	//------------------------------------------
	// 각 Raster 함수 -> 보간 -> Pixel 산출, 출력 
	// Rasterization(oPosition, oColor);		// --> 추후 개량...


	//------------------------------------------
	// 2. 픽셀 파이프라인 (1) 
	//------------------------------------------
	// 픽셀 색상 및 텍스처 혼합
	PixelPipeline1(_기능_생략_);

	//------------------------------------------
	// 3. 픽셀 파이프라인 (2) 
	//------------------------------------------
	// 렌더타겟에 출력전, PS_Main 에서 계산된 색상으로
	// 여러 테스트를 수행하여  최종 출력  픽셀(색상)을 결정합니다.
	// 아래의 각 단계별 수행결과에 따라 , 테스트를 통과하지 못한 픽셀은
	// 전체 파이프라인에서 제외(버려짐) 됩니다.
	//------------------------------------------	 
	// 픽셀 출력 테스트 (AB, AT, DST) 및 최종색 출력(RT)
	PixelPipeline2(_기능_생략_);


	// 이하, 파이프 라인 진행..(대부분 드라이버 상에서 처리)
	// ...


	//임시 그리기.. (여러분의 그리기 함수로 변경) 
	{
	#define v	pos
	#define c	color

			if (m_pso.RState[DOHWARS_FILLMODE] == DOHWAFILL_WIREFRAME)
			{
				//std::cout << "와이어 그리기 \n";
				//와이어프레임  그리기. 
				Line(v[0], v[1], c[0], c[1]);
				Line(v[0], v[2], c[0], c[2]);
				Line(v[1], v[2], c[1], c[2]);
				//std::cout << "와이어 그리기 끝 \n";
			}
			else
			{
				//면 채음 그리기 
				Face(v, c);
				//_Face(v[0], v[1], v[2], c[0], c[1], c[2]);
			}

	#undef v
	#undef c
	}

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 기하 도형 재구성.  
// 
// 이전 스테이지에서 각 (변환된) 정점 데이터를 
// 사용자 지정 기하 형식 (Primitive Topology) 맞추어  "삼각형" 을 재구성합니다.
// 각 VP 및 VS 연산 결과를 조합하여 다음 파이프라인으로 출력합니다.
// 
//  vsouts			기하도형으로 재구성할 (변환된)정점 정보, 배열
//  oPosition		기하도형으로 재구성된 (변환된)정점 위치, 4성분, 배열
//  oColor			기하도형으로 재구성된 정점 색상 (Diffuse Color), 배열
//
int DohwaGraphicsEngine9::PrimitiveAssembly(
	_in_  VSOUTS& vsouts,
	_out_ DOHWAXVECTOR4	oPosition[3],
	_out_ DOHWAXCOLOR		oColor[3]
)
{

	// 이전 스테이지에서 각 (변환된) 정점 데이터를 
	// 사용자 지정 기하 형식 (Primitive Topology) 맞추어  "삼각형" 을 재구성합니다.
	for (UINT i = 0; i < m_input.TopologyVtxSize; i++)
	{
		oPosition[i] = vsouts[i].pos;
		oColor[i] = vsouts[i].diff;
	}

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 픽셀 파이프라인 (#1) : 픽셀 색상 및 텍스처 혼합
//
//
int DohwaGraphicsEngine9::PixelPipeline1()
{

	// 픽셀 셰이더 스테이지 (Pixel Shader Stage)
	// _GS_Main(oPosition, oColor);
	//
	// 또는 ...
	// 
	// 픽셀 고정함수 파이프라인 (Fixed Function Pipeline)
	// _TnL_PP1(oPosition, oColor);


	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 픽셀 파이프라인 (#2) : 픽셀 출력 테스트 (AB, AT, DST) 및 최종색 출력(RT)
// 
// 
int DohwaGraphicsEngine9::PixelPipeline2()
{
	// 깊이 테스트 'Depth-Test' 등의 픽셀 '유효성(Test) 연산... 
	//...SetPixel 에서 처리함.


	// 2.안개 혼합 Fog Blending. 
	// ...

	// 3.가위 테스트 Scissor Test. 
	// ...

	// 4.알파 테스트 Alpha Test. 
	// ... 

	// 5.깊이/스텐실 테스트 Depth/Stencil Test.  
	// ...

	// 6.알파 블렌딩 Alpha Blending. 
	// ...

	// 7.디더링 Dithering. 
	// ...

	// 8.렌더타겟 쓰기 마스크 Rendering Channel Mask  
	// ..

	// 9.렌더타겟에  색상 기록 Write to Render Target.
	//
	//_SetPixel(x, y, oColor);


	// 10.화면 전송 Presentation 
	// ...

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 삼각형 컬링, 2D (Screen) 좌표 기준.   
// 
// 2D Screen 좌표계 정점으로 컬링판정을 수행합니다.
//	기본 컬링 모드는 반시계방향(CCW)으로 이를 만족하면 true 를 리턴합니다. 
// 이 결과는 렌더링 파이프라인에서 해당 기하(삼각형)을 제외하는 조건으로 사용됩니다. 
//				
// warning 2D(Screen) 좌표계는 +Y 축이 화면아랫 방향으로 3D 좌표계와 다름에 주의!
// 
//  v0, v1, v2	삼각형 구성 정점 좌표 (2D)
//  return	컬링 조건 만족시 TRUE, 아니면 FALSE 리턴.
// 
bool DohwaGraphicsEngine9::FaceCulling2(DOHWAXVECTOR2 v0, DOHWAXVECTOR2 v1, DOHWAXVECTOR2 v2)
{
	bool bCull = false;

	switch (m_pso.RState[DOHWARS_CULLMODE])
	{
	case DOHWACULL_NONE:				//컬링 없음. 무조건 그리기.
		bCull = false;
		break;

	case DOHWACULL_CW:				//정점 순서가 시계 방향(CW)이면, 컬링. 그리지 않음.
	{
		DOHWAXVECTOR2 v01 = v1 - v0;	//방향 계산 (연산자 오버로딩 추가)
		DOHWAXVECTOR2 v02 = v2 - v0;

		float z = DOHWAXVec2CCW(&v01, &v02);

		if (z > 0)					//외적의 결과가 양수면, 'CW' 				
			bCull = true;			//곧, 컬링되어야 함.	  
	}
	break;

	case DOHWACULL_CCW:				//정점 순서가 반시계 방향(CCW)이면, 컬링. 그리지 않음.
	{
		DOHWAXVECTOR2 v01 = v1 - v0;
		DOHWAXVECTOR2 v02 = v2 - v0;

		float z = DOHWAXVec2CCW(&v01, &v02);

		if (z < 0)					//외적의 결과가 음수면, 'CCW' 
			bCull = true;			//곧, 컬링되어야 함.	
	}
	break;
	}


	return bCull;
}

////////////////////////////////////////////////////////////////////////////////
//
// 삼각형 컬링, 3D (Device / NDC Space) 좌표 기준.  
// 
// 3D 좌표계 정점으로 컬링판정을 수행합니다.
// 기본 컬링 모드는 반시계방향(CCW)으로 이를 만족하면 true 를 리턴합니다. 
// 이 결과는 렌더링 파이프라인에서 해당 기하(삼각형)을 제외하는 조건으로 사용됩니다. 
// 
// \warning DX 의 3D 좌표계는 (특별한 조건이 없다면) +Y 축이 화면 윗 방향입니다.
//			 2D 화면 좌표계와 다름에 주의 하십시오!
// 
// v0, v1, v2	삼각형 구성 정점 좌표 (3D)
// return	컬링 조건 만족시 TRUE, 아니면 FALSE 리턴.
//
bool DohwaGraphicsEngine9::FaceCulling3(DOHWAXVECTOR2 v0, DOHWAXVECTOR2 v1, DOHWAXVECTOR2 v2)
{
	bool bCull = false;

	switch (m_pso.RState[DOHWARS_CULLMODE])
	{
	case DOHWACULL_NONE:				//컬링 없음. 무조건 그리기.
		bCull = false;
		break;

	case DOHWACULL_CW:				//정점 순서가 시계 방향(CW)이면, 컬링. 그리지 않음.
	{
		DOHWAXVECTOR2 v01 = v1 - v0;	//방향 계산 (연산자 오버로딩 추가)
		DOHWAXVECTOR2 v02 = v2 - v0;

		float z = DOHWAXVec2CCW(&v01, &v02);

		if (z < 0)					//외적의 결과가 음수면, 'CW' ★			
			bCull = true;			//곧, 컬링되어야 함.	  
	}
	break;

	case DOHWACULL_CCW:				//정점 순서가 반시계 방향(CCW)이면, 컬링. 그리지 않음.
	{
		DOHWAXVECTOR2 v01 = v1 - v0;
		DOHWAXVECTOR2 v02 = v2 - v0;

		float z = DOHWAXVec2CCW(&v01, &v02);

		if (z > 0)					//외적의 결과가 양수면, 'CCW' ★
			bCull = true;			//곧, 컬링되어야 함.	
	}
	break;
	}


	return bCull;
}

////////////////////////////////////////////////////////////////////////////////
//
// 뷰포트 멥핑 (Viewport) 
// 
// 정규장치공간(NDC) 의 정점을 출력 화면 좌표(Screen) 로 변환합니다.  
// 입력된 좌표는 해상도에 맞게 보정(Scale & Bias) 되어 다음 파이프라인으로 전달됩니다. 
// 
// 화면 좌표로 변환 : 해상도 (800x600) 기준
//  pos.x = -1 ~ 1 -->  0 ~ 799
//	pos.y = -1 ~ 1 -->  0 ~ 599
//
//
//  iPos	좌표 입력. 3성분.
//  oPos	좌표 출력. 2성분.
//
int DohwaGraphicsEngine9::Viewport(
	DOHWAXVECTOR3 iPos[3],
	DOHWAXVECTOR2 oPos[3]
)
{
	#define vPos  		iPos[i]						//가독성 향상을 위한 메크로 선언.
	#define vScreen 	oPos[i]					

		float w = (float)m_om.PresentParam.Width;
		float h = (float)m_om.PresentParam.Height;

		for (UINT i = 0; i < m_input.TopologyVtxSize; i++)	//기하도형별 정점수 만큼 처리.
		{
			vScreen.x = vPos.x * (w / 2) + (w / 2);
			vScreen.y = -vPos.y * (h / 2) + (h / 2);
			//vScreen.z =  vPos.z;   <-- 일단 생략..
		}

	#undef vPos
	#undef vScreen

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
//  정점 처리 파이프라인
// 
// 각 Vertex 별 연산 Operation - 정점별 변환, 조명, 안개 등의 연산을 처리합니다.  
//   - 정점 변환 (World Transform)  
//   - 정점 블랜딩 (Vertex Blending)  
//   - 정점 변환 (View Transform)  
//   - 정점 안개 (Vertex Fog)  
//   - 정점 조명 (Vertex Lighting)  
//   - 정점 변환 (Projection Transform)  
//    
// 출력결과는 기하 파이프라인 (Geometry Pipeline, GP) 으로 넘어감니다.  
// 
// pos	(처리할) 현재 정점 좌표, 3성분 
// col	(처리할) 현재 정점 색상 (Diffuse Color)
//
VS_OUTPUT DohwaGraphicsEngine9::TnL_VP(
	DOHWAXVECTOR3 pos,
	DOHWAXCOLOR	col
)
{
	VS_OUTPUT output;

	// Dohwa.Pos -> D3D.Pos 로 변환. 
	// D3DXVECTOR4 vPos( pos.x, pos.y, pos.z, 1);			//<DX>
	// Dohwa 벡터 변수 사용.
	DOHWAXVECTOR4 vPos(pos.x, pos.y, pos.z, 1);				//<Dohwa>★

	//1. 정점, 월드 변환 (World Transform)
	//   vPos = vPos * m_mWorld;
	//D3DXVec4Transform(&vPos, &vPos, &m_mWorld);		//단위 변환 <DX>★		
	DOHWAXVec4Transform(&vPos, &vPos, &m_tms.mTFM[DOHWATS_WORLD]);			//단위 변환 <Yena>★	
	//* 정점 블랜딩 (Vertex Blending)
	//...

	//2. 정점, 뷰 변환 (View Transform)
	//   vPos = vPos * m_mView;
	//D3DXVec4Transform(&vPos, &vPos, &m_mView);		//단위 변환.<DX>★			
	DOHWAXVec4Transform(&vPos, &vPos, &m_tms.mTFM[DOHWATS_VIEW]);			//단위 변환.<Yena>★

	//3. 정점 안개 (Vertex Fog)
	//...

	//4. 정점 조명 (Vertex Lighting)
	//...

	//5. 정점, 투영 변환 (Projection Transform)
	//   vPos = vPos * m_mProj;
	//D3DXVec4Transform(&vPos, &vPos, &m_mProj);		//단위 변환.<DX>★				
	DOHWAXVec4Transform(&vPos, &vPos, &m_tms.mTFM[DOHWATS_PROJECTION]);			//단위 변환.<Yena>★

	//외부로 출력
	//...
	output.pos = vPos;			//4성분 출력, 주의.	
	output.diff = col;

	return output;
}

////////////////////////////////////////////////////////////////////////////////
//
// 기하 파이프라인 [고정 함수][TnL]  
// 
// 각 Face 별 연산 Operation 을 처리합니다.  
//   - 삼각형 컬링 (Face Culling)  
//   - 사용자 평면 클립핑 (User Plane Clipping)   
//   - 시야 공간 클립핑 (Viewing Frustum Clipping)   
//   - 동차 나누기 (Homogeneous Divide)  
//   - 뷰포팅 (View Port) (3D->2D)   
// 
// 출력결과는 픽셀 파이프라인 (Pixel Pipeline, PP) 으로 넘어감니다.  
// 
// <DX9> 고정함수 파이프라인만 지원. DX10 이상, 기하 셰이더 (Geometry Shader) 지원  
// <Dohwa> 고정함수 파이프라인만 지원.  
// 
// [in]	vPos4		(입력) 정점 좌표, 4성분 ★
// [out]	oPos		(출력) 정점 좌표, 3성분 ★
// [in, out]	oColor	(출력) 정점색 (Diffuse Color)
//
int DohwaGraphicsEngine9::TnL_GP(
	_in_	DOHWAXVECTOR4 iPos4[3],
	_out_	DOHWAXVECTOR3 oPos[3],
	_inout_ DOHWAXCOLOR	oColor[3]
)
{
	DOHWAXVECTOR2 vScreen[3];			//화면(공간)좌표(2D)
	//#define vPos vPos4
	//#define oPos oPos

	//1. 삼각형 컬링 (Face Culling)
	if (m_input.TopologyVtxSize >= 3)						//점/선은 뒷면컬링 없음.
	{
		if (FaceCulling3(iPos4[0], iPos4[1], iPos4[2]))		//3D 버전.
		{
			//true 가 리턴되면, 컬링해야 하는 삼각형으로 판정, 
			//다음 삼각형으로 넘어간다.
			return DOHWA_CULLED;						//조건 충족시, 그리지 않음.
		}
	}

	// 2.사용자 평면 클립핑 (User Plane Clipping)  
	//...생략...
	// return YN_CLIPPED;							//조건 충족시, 그리지 않음.


	// 3.시야 공간 클립핑 (Viewing Frustum Clipping) 
	//...생략...
	// return YN_CILPPED;							//조건 충족시, 그리지 않음.


	// 4. 동차 나누기 (Homogeneous Divide)
	//...

	//동차 변환 후 3D 가 되었다 가정...(벡터 형변환 연산자 오버로딩 필요) ★
	DOHWAXVECTOR3 vPos3[3];
	vPos3[0] = iPos4[0];
	vPos3[1] = iPos4[1];
	vPos3[2] = iPos4[2];


	// 5. 뷰포팅 (View Port) (3D->2D) ★
	//
	Viewport(vPos3, vScreen);



	// 6. 최종 결과 출력 : PS 로 출력.
	//
	for (UINT i = 0; i < m_input.TopologyVtxSize; i++)	//기하도형별 정점개수만큼 처리.
	{ 												//해상도 800x600 기준.
		oPos[i].x = vScreen[i].x;					//화면좌표.. ( x: 0 ~ 799)
		oPos[i].y = vScreen[i].y;					//           ( y: 0 ~ 599)
		//oPos[i].z = vPos3[i].z;					//깊이값 Depth ( z : 0 ~ 1.0f) : ..생략...★
		oColor[i] = oColor[i];						//색상은 변환없이 그대로..
	}


#undef vPos
#undef oPos
	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 픽셀 처리 파이프라인 [고정함수][TnL]  
// 
// 픽셀 Pixel 별 연산 Operation 을 처리하는 중요 스테이지(함수) 중 하나입니다.  
//   - 텍스처 혼합 (Texture Stage)  
//   - 조명 혼합 (Specular Light Blending)  
//   - 픽셀 안개 혼합 (Pixel Fog Blending)  
// 
// 색상 출력은 필수이며 연산 결과는 렌더타겟(RenderTarget, RT) 에 기록됩니다.   
// 
// <DX> 셰이더는 레거시 고정 함수 파이프라인(Fixed Function Pipeline) 의 기능을 사용자의 
// 선택에 따라 조정하거나 변경할 수 있습니다. 또한 픽셀 셰이더(Pixel Shader, PS) 를 통해 
// 여러분만의 기능으로 독자적인 파이프라인 구축이 가능합니다. 
// 픽셀 셰이더(Pixel Shader, PS) 는 GPU 프로그래밍 언어(HLSL) 로 개발되며 사용자 연산을 처리할 수 있습니다.  
// 
// <DX9> 고정 함수 파이프라인 및 픽셀 셰이더 지원  
// <Dohwa> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.
//        더하여  PS 및 HLSL 형식의 개발환경도 함께 제작합니다.
// 
// pos		픽셀 좌표, 배열
// color	픽셀 색상, 배열
//
int DohwaGraphicsEngine9::TnL_PP(
	_in_ DOHWAXVECTOR2 Pos[3],
	_in_ DOHWAXCOLOR	 Color[3]
)
{
	//1. 텍스처 혼합 (Texture Stages)
	//...

	//2. 정반사광 혼합 (Add Specular)
	//...

	//3. 픽셀 안개 혼합 (Pixel Fog Blending)
	//...	 

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// Dohwa Graphics Engine : Rendering State API
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태 초기화. 
//
void DohwaGraphicsEngine9::InitRenderState()
{
	::ZeroMemory(m_pso.RState, sizeof(DWORD) * DOHWARS_MAX_);

	// 렌더링 장치, 상태 기본값 설정
	//
	m_pso.RState[DOHWARS_FILLMODE] = DOHWAFILL_SOLID;	//단일색 채우기 
	m_pso.RState[DOHWARS_CULLMODE] = DOHWACULL_CCW;		//반시계 방향(CCW) 컬링 
}

///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 조절.
//
int DohwaGraphicsEngine9::SetRenderState(DOHWARENDERSTATETYPE state, DWORD value)
{
	m_pso.RState[state] = value;

	return DOHWA_OK;
}
///////////////////////////////////////////////////////////////////////////////
//
//! 렌더링 상태 얻기   
//
int DohwaGraphicsEngine9::GetRenderState(DOHWARENDERSTATETYPE state, DWORD* value)
{
	*value = m_pso.RState[state];

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 변환 행렬 설정 
//
int DohwaGraphicsEngine9::SetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* m)
{
	m_tms.mTFM[ts] = *m;

	return DOHWA_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
// 변환 행렬 얻기 ★
//
int DohwaGraphicsEngine9::GetTransform(DOHWATRANSFORMSTATETYPE ts, DOHWAXMATRIX* m)
{
	*m = m_tms.mTFM[ts];

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// Dohwa Graphics Engine : Drawing API
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// 라인 그리기. (ver.GDI) 직접 그리기.
// 
//  v0, v1	라인 구성 정점 좌표
//  c0, c1	라인 구성 정점 색상 
//	<주> 정점 버퍼 내의 색상은 여전히 DWORD 형임에 주의, 클래스 연산자 오버로딩으로 호환성 유지.
// 
//
int DohwaGraphicsEngine9::Line(
	DOHWAXVECTOR2 v0, DOHWAXVECTOR2 v1,
	DOHWAXCOLOR   c0, DOHWAXCOLOR   c1
)
{
	//브레젠험 알고리즘 Bresenham's Algorithm

	//직선의 너비와 높이를 구한다.
	int width = v1.x - v0.x;
	int height = v1.y - v0.y;

	//너비와 높이의 절댓값을 구한 후 이를 비교해 경사도를 구해 저장
	bool isGradualSlope = (std::abs(width) >= std::abs(height)); //true : 급한, false : 완만

	//x축, y축의 진행 방향을 파악
	int dx = (width >= 0) ? 1 : -1;
	int dy = (height >= 0) ? 1 : -1;

	//판별식을 위한 수 계산 (판별을 위해서는 값이 모두 양수여야함)
	int fw = dx * width;
	int fh = dy * height;

	//최초로 사용할 판별식 f를 지정
	// 판별식 D = 2 dy - dx
	// 경사가 급한 경우 기본 판별식 사용
	// 경사가 완만한 경우 x 축과 y축을 뒤집어서 계산

	int f = isGradualSlope ? 2 * fh - fw : 2 * fw - fh;

	//D <= 0
	//선이 변화 없이 진행하는 경우 판별식 조정 값 지정
	int f1 = isGradualSlope ? 2 * fh : 2 * fw;

	// D > 0
	//선의 진행이 변화하는 경우 판별식 조정 값 지정
	int f2 = isGradualSlope ? 2 * (fh - fw) : 2 * (fw - fh);

	//최초 선 그리기를 시작할 지점
	int x = v0.x;
	int y = v0.y;

	int xend = v1.x;
	int yend = v1.y;

	if (isGradualSlope)
	{
		while (x != xend)
		{

			DOHWAXVECTOR2 v0p = DOHWAXVECTOR2{ (float)x, (float)y } - DOHWAXVECTOR2(v0);
			DOHWAXVECTOR2 v0v1 = DOHWAXVECTOR2(v1) - DOHWAXVECTOR2(v0);
			float t = DOHWAXVec2Dot(&v0p, &v0v1) / DOHWAXVec2Dot(&v0v1, &v0v1);

			DOHWAXCOLOR color;
			DOHWAXLerp(&color, c0, c1, t);
			SetPixel(x, y, (DWORD)color);

			if (f <= 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				y += dy;
			}

			x += dx;
		}
	}
	else
	{
		while (y != yend)
		{
			DOHWAXVECTOR2 v0p = DOHWAXVECTOR2{ (float)x, (float)y } - DOHWAXVECTOR2(v0);
			DOHWAXVECTOR2 v0v1 = DOHWAXVECTOR2(v1) - DOHWAXVECTOR2(v0);
			float t = DOHWAXVec2Dot(&v0p, &v0v1) / DOHWAXVec2Dot(&v0v1, &v0v1);
			DOHWAXCOLOR color;
			DOHWAXLerp(&color, c0, c1, t);
			//std::cout << "inner2 : " << x << " : " << y << "\n";
			SetPixel(x, y, (DWORD)color);

			if (f <= 0)
			{
				f += f1;
			}
			else
			{
				f += f2;
				x += dx;
			}

			y += dy;
		}
	}
	return DOHWA_OK;

}




///////////////////////////////////////////////////////////////////////////////
//
// 라인 그리기. (ver.GDI) 
//
int DohwaGraphicsEngine9::Line(DOHWAXVECTOR2& v0, DOHWAXVECTOR2& v1, DOHWAXCOLOR& col)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, col);
	HPEN hOldPen = (HPEN)SelectObject(m_om.hRT, hPen);
	MoveToEx(m_om.hRT, (int)v0.x, (int)v0.y, NULL);
	LineTo(m_om.hRT, (int)v1.x, (int)v1.y);
	SelectObject(m_om.hRT, hOldPen);
	DeleteObject(hPen);
	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 삼각형 그리기 : 정점색 추가.(DOHWAXCOLOR 타입)(ver.GDI) 
// 
// <주> 정점 버퍼 내의 색상은 여전히 DWORD 형임에 주의, 클래스 연산자 오버로딩으로 호환성 유지.
// 
// v0, v1, v2	삼각형 구성 정점 좌표
// c0, c1, c2	삼각형 구성 정점 색상
//
int DohwaGraphicsEngine9::Face(
	DOHWAXVECTOR2 pos[3],
	DOHWAXCOLOR   col[3]
)
{
	//! \todo2 <과제> 삼각형 그리기 ★
	//! 정점 3개와 색상 3개를 이용해 삼각형에 색상을 채웁니다.★
	// ...
	//
	// 함수 시그니쳐(Signature : 인자/리턴값/이름) 의 변경없이,
	// 함수의 Body 를 완성 하십시요.★
	// ...

	// <GDI 버전> 임시 그리기.. (삭제후 과제 수행)★
	// ...
	#define v0 pos[0]
	#define v1 pos[1]
	#define v2 pos[2]
	#define c0 col[0]
	#define c1 col[1]
	#define c2 col[2]

		POINT pt[3] = { v0, v1, v2 };	//연산자 오버로딩.
		//Pixel Space 계산 left, top, right, bottom
		int pixel_space[4] = { INT_MAX, INT_MAX, 0, 0 };
		DOHWAXCOLOR color(255.0f, 255.0f, 255.0f, 255.0f);

		for (int i = 0; i < 3; ++i)
		{
			if (pixel_space[0] >= pt[i].x) pixel_space[0] = pt[i].x;
			if (pixel_space[1] >= pt[i].y) pixel_space[1] = pt[i].y;
			if (pixel_space[2] <= pt[i].x) pixel_space[2] = pt[i].x;
			if (pixel_space[3] <= pt[i].y) pixel_space[3] = pt[i].y;
		}

		//무게 중심 좌표를 활용해 삼각형 내외부 판단
		float D = (pt[1].y - pt[2].y) * (pt[0].x - pt[2].x) + (pt[2].x - pt[1].x) * (pt[0].y - pt[2].y);

		if (D == 0.0f) return 0; //세점이 일직선이라 그릴 수 없는 경우

		for (int x = pixel_space[0]; x <= pixel_space[2]; ++x)
		{
			for (int y = pixel_space[1]; y <= pixel_space[3]; ++y)
			{
				float rambda1 = ((pt[1].y - pt[2].y) * (x - pt[2].x) + (pt[2].x - pt[1].x) * (y - pt[2].y)) / D;
				if (rambda1 < 0.0f) continue;

				float rambda2 = ((pt[2].y - pt[0].y) * (x - pt[2].x) + (pt[0].x - pt[2].x) * (y - pt[2].y)) / D;
				if (rambda2 < 0.0f) continue;

				float rambda3 = 1 - rambda1 - rambda2;
				if (rambda3 < 0.0f) continue;

				color = c0 * rambda1 + c1 * rambda2 + c2 * rambda3;

				SetPixel( x, y, (DWORD)color);
			}
		}


	#undef v0
	#undef v1
	#undef v2
	#undef c0
	#undef c1
	#undef c2

	return DOHWA_OK;
}

///////////////////////////////////////////////////////////////////////////////
//
// 수평선 그리기   
// 지정 위치와 색상으로 수평선을 출력합니다  
// (ex) v0(x1, y) --> v1(x2, y)
// 
// 픽셀 출력에 사용되는  SetPixel 로 인해 상당한 성능 저하가 발생됨.
// 다음 버전에서 개량 예정. 
//
// hdc	출력 화면 DC (렌더타겟)
// x1	수평선 시작 점, x 좌표
// x2	수평선 끝 점, x 좌표
// y	수평선, y 좌표
// c0	수평선 시작 점 색상
// c1	수평선 끝 점 색상
//
void DohwaGraphicsEngine9::HLine(HDC hdc, int x1, int x2, int y, DOHWAXCOLOR c0, DOHWAXCOLOR c1)
{
	//...
}

////////////////////////////////////////////////////////////////////////////////
//
// 렌더 타겟에 픽셀 출력  
// 
// 우리는 직접 읽기/쓰기 가능한 메모리-비트멥(DIB)을 사용하고 있습니다.  
// DIB 의 컬러비트가 일반적인 GDI 의 것과 반대인 것에 주의하십시오.  
//  - DDI : color = 0x00 BB GG RR  
//  - DIB : color = 0x00 RR GG BB   
// 
// 장치 독립 비트멥(Device-Independent Bitmap) 도움말은 아래의 문서를 참고하십시오.  
// [참고1] https://msdn.microsoft.com/ko-kr/library/windows/desktop/dd183562(v=vs.85).aspx  
// [참고2] https://msdn.microsoft.com/ko-kr/library/ff566496(v=vs.85).aspx  
// 
//  x		출력 화면 좌표(x)
//  y		출력 화면 좌표(y)
//  color	출력 색상
//
void DohwaGraphicsEngine9::SetPixel(int x, int y, DOHWAXCOLOR color)
{
	::SetPixel(m_om.hRT, x, y, color);		//GDI 기록하기.
}

///////////////////////////////////////////////////////////////////////////////
//
// Dohwa Graphics Engine : Drawing API (외부)
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
// 그래픽 엔진, 그리기 선행 준비.  
// 
// 렌더링 전 디바이스에 공급된 개별 데이터를 재구성합니다.	
// 렌더링 각 단계별 반복적으로 계산되는 상수 데이터를 미리 계산하여 파이프라인의 연산 속도 개선이 목적.
// 
//  PrimitiveType	렌더링 기하 타입.
//  StartVertex		렌더링할 정점 시작번호 (정점버퍼 안)
//  PrimitiveCount	렌더링할 기하 개수.
//
int DohwaGraphicsEngine9::PreDraw(
	DOHWAPRIMITIVETYPE PrimitiveType,
	UINT StartVertex,
	UINT PrimitiveCount
	//B3YDBGINFO* pDbgInfo
)
{


	//------------------------------------------------------------
	// 그래픽 엔진 리셋
	//------------------------------------------------------------
	//Reset();

	//------------------------------------------------------------
	// 장치 정보 연결 및 필요 정보 복사.
	//------------------------------------------------------------
	//m_pDev = pDev;
	m_input.PrimitiveType = PrimitiveType;
	m_input.PrimitiveCount = PrimitiveCount;
	m_input.StartVertex = StartVertex;

	//m_pDbgInfo = pDbgInfo;



	//------------------------------------------------------------
	// 렌더링 전 디바이스에 공급된 개별 데이터를 재구성하여
	// 렌더링 각 단계별 반복적으로 계산되는 상수 데이터를 
	// 미리 계산합니다.  파이프라인의 연산 속도 개선이 목적.
	//------------------------------------------------------------
	//조명, 행렬, 기타 상수들 사전 계산. 
	//_PreCompute();		



	//------------------------------------------------------------
	// 한번에 처리할 정점개수 지정 
	// 기하도형의 형태 PrimitiveType 에 따라 다르게 처리.
	// Triangle-List = 3 개.
	// Line-List = 2개 
	//------------------------------------------------------------
	switch (m_input.PrimitiveType)
	{
	case DOHWAPT_LINELIST: m_input.TopologyVtxSize = 2;
		break;
	default:
	case DOHWAPT_TRIANGLELIST: m_input.TopologyVtxSize = 3;
		break;
	}


	//------------------------------------------------------------
	// 디버깅 정보 초기화. 
	//------------------------------------------------------------
	///m_DbgInfo.Clear();

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 그래픽 엔진, 렌더링 시작.
// 
// 렌더링 파이프라인의 각 단계별 연산(VP/GP/PP)이 처리되며, 최종 결과는 렌더 타겟에 출력됩니다.  
//   - VertexPipeline  
//   - GeometryPipeline  
//   - PixelPipeline  
// 
// 이 메소드는 그래픽 엔진의 핵심 기능을 수행합니다. 함수 호출 전에 정점 버퍼, FVF, 정점 크기(Stride) 같은
// 렌더링에 필요한 모든 정보가 디바이스에 등록되어 있어야 합니다.
//
// 렌더링 장치와 자원
//		일반적인 하드웨어 렌더링 장치(Device) 는 다양한 렌더링 객체와 자원을 사용합니다.  
//   	- 기하 버퍼 : VBs, IB, UAVs,...  
//   	- 변환 정보 : Matrices, CBs  
//   	- 조명, 텍스처 : Lights, Materials, Textures, Samplers  
//   	- 파이프라인 상태 객체 : PSOs   
//		- 셰이더 : Shader
// 	렌더링 장치(Device)는 그리기가 시작되면 "현재" 등록(설정)된 자원과 객체들로 연산을 수행합니다.  
// 	따라서 필요 자원들은 렌더링 시작 전 장치에 등록되어야 합니다.  
// 
// DX9 렌더링 파이프라인
//		DX9 은 두가지 렌더링 파이프라인을 제공합니다.
//		- 고정 함수 파이프라인 (Fixed Function Pipeline 또는 TnL) 
//		- 셰이더 파이프라인 (Shader 또는 Programmable Function Pipeline) 
// 
//		사용자는 렌더링 목적과 성능에 맞추어 이들을 조정하거나 변경할 수 있습니다.
//		셰이더 파이프라인은 셰이더(Shader) 를 통해 여러분만의 기능으로 독자적인 파이프라인 구축도 가능합니다.
//		셰이더(Shader) 는 GPU 사용자 연산 프로그램이며 GPU 프로그래밍 언어(HLSL, GLSL, DXIL, SPIR-V) 로 개발됩니다.
// 
// <Dohwa> TnL 파이프라인을 우선 제작하고, 추후 셰이더 파이프라인을 구축합니다.  
//				 더하여 VS, PS 및 HLSL 형식의 개발환경도 함께 제작합니다.
// 
//
int DohwaGraphicsEngine9::Draw()
{
	if (DOHWA_INVALIED(m_input.pVB[0])) return DOHWA_FAIL;
	if (DOHWA_INVALIED(m_input.Stride)) return DOHWA_FAIL;


	//연산결과 입/출력용 변수들.
	DOHWAXVECTOR3 vPos[3];					//정점 좌표 (3성분)
	DOHWAXVECTOR3 oPos3[3];					//정점 좌표 (3성분)
	DOHWAXVECTOR4 oPos4[3];					//정점 좌표 (4성분)
	DOHWAXCOLOR	oColor[3];					//색상 출력.(Diffuse Color)


	//현재 처리중인 기하정보 (디버깅용)
	m_stm.CurrVtxIndex = m_input.StartVertex;			//시작, 정점 번호 설정 (외부 지정)
	m_stm.CurrPrimIndex = -1;					//기하 도형 번호 (Line, Face) (초기화)
	m_stm.CurrPrimDrawCnt = 0;					//처리한 기하도형 개수(Line, Face) (초기화)

	int res = DOHWA_OK;

	//------------------------------------------------------------
	// 각 렌더링 파이프라인별로 적절한 연산을 수행한 후, 
	// 그 결과를 다음 파이프라인으로 공급합니다.
	// 
	// 각 변환에 필요한 데이터는 DrawPrimitive 함수가 호출되기 전에
	// Device 에 공급되어야 합니다. 
	//------------------------------------------------------------
	while (1)
	{
		//1. 정점 읽기 : "Vertex Streaming" ★
		res = VertexStreaming(vPos, oColor);
		if (DOHWA_FAILED(res))
		{
			//작업 완료, 또는 에러..
			break;
		}

		//2. 정점 조립 : 다중 정점 버퍼 조합 운용 (생략) ★
		//_VertexAssembly(_기능_생략_);		


		//3. 정점 파이프라인 처리
		VertexPipeline(vPos, oPos4, oColor);


		//4. 기하 파이프라인 처리...★
		res = GeometryPipeline(oPos4, oPos3, oColor);
		if (DOHWA_FAILED(res))
		{
			//특정 조건 부합시, 그리지 않음. ★
			//... 컬링, 클립핑 등등...
		}
		else
		{
			//5. 픽셀 파이프라인 처리...★
			PixelPipeline(oPos3, oColor);

			//기하도형(Line, Face) 1개 출력 완료.
			//다음 기하 준비...

			//디버깅 정보 갱신 : 그리기 성공한 Primtivie 정보 수집...
			//m_DbgInfo.Update(m_input.PrimTypeVtxSize, 1);
		}


		//기하도형(Line, Face) 1개 출력 완료.
		//다음 기하도형 처리..
		{
			m_stm.CurrVtxIndex += m_input.TopologyVtxSize;					//다음 정점 색인 계산.
			m_stm.CurrPrimIndex = m_stm.CurrVtxIndex / m_input.TopologyVtxSize;	//다음 기하 색인 계산. 

			//지정 개수 이상의 삼각형이 그려지면, 작업 종료... 			
			if (++m_stm.CurrPrimDrawCnt >= m_input.PrimitiveCount)
				break;

			//임시 버퍼 비우기
			ZeroMemory(vPos, sizeof(DOHWAXVECTOR3) * 3);
			ZeroMemory(oPos3, sizeof(DOHWAXVECTOR3) * 3);
			ZeroMemory(oPos4, sizeof(DOHWAXVECTOR4) * 3);
			ZeroMemory(oColor, sizeof(DOHWAXCOLOR) * 3);
		}
	}


	//디버깅 정보 갱신 : 전체장면(Scene) 정보들. 
	//m_DbgInfo.Update();


	//임시버퍼 제거.
	//_TempBufferRelease();

	return DOHWA_OK;
}



//////////////////////////////////////////////////////////////////////////////
//
//! 그래픽 엔진, 그리기 후위 작업, 임시 버퍼 제거 등.
//
int DohwaGraphicsEngine9::PostDraw()
{
	//...

	return DOHWA_OK;
}