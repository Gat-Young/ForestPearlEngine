#pragma once
#include <Windows.h>


////////////////////////////////////////////
//
// DOHWA 버퍼 규격 : D3DFORMAT 대응 <DOHWA> 미지원 기능 생략
//
enum DOHWAFORMAT
{
	DOHWAFMT_UNKNOWN = 0,

	// 렌더타겟용 포멧들.
	DOHWAFMT_R8G8B8               = 20,
	DOHWAFMT_A8R8G8B8				= 21,
	DOHWAFMT_X8R8G8B8             = 22,
	DOHWAFMT_R5G6B5               = 23,
	DOHWAFMT_X1R5G5B5             = 24,
	DOHWAFMT_A1R5G5B5             = 25,
	DOHWAFMT_A4R4G4B4             = 26,
	DOHWAFMT_R3G3B2               = 27,
	DOHWAFMT_A8                   = 28,
	DOHWAFMT_A8R3G3B2             = 29,
	DOHWAFMT_X4R4G4B4             = 30,
	DOHWAFMT_A2B10G10R10          = 31,
	DOHWAFMT_A8B8G8R8             = 32,

	// Depth/Stencil Buffer 용 포멧들 : <DX> 실수표현의 정밀도 별 포멧 결정.(성능 및 호환성 고려)
	//								 : <DOHWA> 32bit 만 지원.
	DOHWAFMT_D16_LOCKABLE         = 70,
	DOHWAFMT_D32					= 71,		//32bIT Depth Buffer. <DX9> 미지원  <DOHWA> 이것만 사용. 
	DOHWAFMT_D24S8                = 75,		//24bit Dpeth + 8bit Stencil :  (DX9)
	DOHWAFMT_D24X8                = 77,		//24bit Depth Only. (DX9) 
	DOHWAFMT_D16                  = 80,		//16bit Depth Only. (DX9)

	// Vertex Buffer, Index Buffer 용 포멧
	// <DOHWA> 시스템 메모리를 사용하므로 이 포멧은 유의미한 동작은 없으나, 구조의 일관성을 위해 사용.
	DOHWAFMT_VERTEXDATA = 100,
	DOHWAFMT_INDEX16  = 101,
	DOHWAFMT_INDEX32 = 102,

	DOHWAFMT_FORCE_DWORD = 0x7fffffff
};

///////////////////////////////////////////////////////////////////
//
// DOHWA 자원 규격 : D3DRESOURCETYPE 대응, <DOHWA> 미지원 기능 생략.
//
enum DOHWARESOURCETYPE {
	DOHWARTYPE_SURFACE = 1,				//일반 "서피스" 버퍼
	DOHWARTYPE_VOLUME = 2,
	DOHWARTYPE_TEXTURE = 3,				//!< 텍스처용
	DOHWARTYPE_VOLUMETEXTURE = 4,
	DOHWARTYPE_CUBETEXTURE = 5,
	DOHWARTYPE_VERTEXBUFFER = 6,			//!< 정점 버퍼용
	DOHWARTYPE_INDEXBUFFER = 7,			//!< 색인 버퍼용

	DOHWARTYPE_FORCE_DWORD = 0x7fffffff
};

///////////////////////////////////////////////////////////////////
//
// DOHWA Memory Pool 종류 : D3DPOOL 대응 <DOHWA> 미지원 기능 생략
//
enum DOHWAPOOL {
	DOHWAPOOL_DEFAULT = 0,				//기본 GPU 메모리(힙) 사용
	DOHWAPOOL_MANAGED = 1,				//(자동)관리되는 GPU 메모리 사용 <DX 권장 옵션>
	DOHWAPOOL_SYSTEMMEM = 2,			//시스템 메모리 사용 <DOHWA 옵션>
	DOHWAPOOL_SCRATCH = 3,

	DOHWAPOOL_FORCE_DWORD = 0x7fffffff
};

////////////////////////////////////////////////////////////////////
//
// DOHWA 버퍼 용도 정의 : D3DUSAGE_XXX 대응
//
// Usages for RT, DS.
#define DOHWAUSAGE_RENDERTARGET       (0x00000001L)
#define DOHWAUSAGE_DEPTHSTENCIL       (0x00000002L)
#define DOHWAUSAGE_DYNAMIC            (0x00000200L)

// Usages for Texture
#define DOHWAUSAGE_AUTOGENMIPMAP    (0x00000400L)
#define DOHWAUSAGE_DMAP             (0x00004000L)

// Usages for Vertex/Index buffers 
#define DOHWAUSAGE_WRITEONLY          (0x00000008L)
#define DOHWAUSAGE_SOFTWAREPROCESSING (0x00000010L)

////////////////////////////////////////////////////////////////////
//
// DOHWA 기하도형 타입 : D3DPRIMITIVETYPE 대응, <DOHWA> 미지원 기능 생략
//
enum DOHWAPRIMITIVETYPE
{
	DOHWAPT_POINTLIST           = 1,
	DOHWAPT_LINELIST = 2,
	DOHWAPT_LINESTRIP           = 3,
	DOHWAPT_TRIANGLELIST = 4,
	DOHWAPT_TRIANGLESTRIP       = 5,
	DOHWAPT_TRIANGLEFAN         = 6,
	DOHWAPT_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};

/////////////////////////////////////////////////////////////////////////////// 
//
//! 변환 상태 설정 타입. : D3DTRANSFORMSTATETYPE  대응.
//
enum DOHWATRANSFORMSTATETYPE
{
	DOHWATS_NONE = 0,
	DOHWATS_WORLD = 1,		//월드 변환 : 아래 주석블럭 [도움2] 참고
	DOHWATS_VIEW = 2,		//뷰 변환.
	DOHWATS_PROJECTION = 3,		//투영 변환.
	DOHWATS_TEXTURE0      = 16,       //텍스처 변환. 이하 추후 사용.
	DOHWATS_TEXTURE1      = 17,
	DOHWATS_TEXTURE2      = 18,
	DOHWATS_TEXTURE3      = 19,
	DOHWATS_TEXTURE4      = 20,
	DOHWATS_TEXTURE5      = 21,
	DOHWATS_TEXTURE6      = 22,
	DOHWATS_TEXTURE7      = 23,
	
	DOHWATS_MAX_
};

//! [도움2] DOHWATS_WORLD 매크로 정의 : D3DTS_WORLD 대응.★
//! : DX 에서는 아래와 같이 정의 되어있으나 (Vertex Blending 용) 
//!   우리에게는 불필요, 사용 안 함.
//#define DOHWATS_WORLDMATRIX(index) (B3YTRANSFORMSTATETYPE)(index + 256)
//#define DOHWATS_WORLD  B3YTS_WORLDMATRIX(0)
//#define DOHWATS_WORLD1 B3YTS_WORLDMATRIX(1)
//#define DOHWATS_WORLD2 B3YTS_WORLDMATRIX(2)
//#define DOHWATS_WORLD3 B3YTS_WORLDMATRIX(3)

/////////////////////////////////////////////////////////////////////////////// 
//
// DOHWA 렌더링 상태 옵션 : D3DRENDERSTATETYPE 대응
//
enum DOHWARENDERSTATETYPE {
	DOHWARS_ZENABLE                   = 7,
	DOHWARS_FILLMODE				  = 8,    // Fill Mode State.★
	DOHWARS_ZWRITEENABLE              = 14,   // TRUE to enable z writes 
	DOHWARS_ALPHATESTENABLE           = 15,   // TRUE to enable alpha tests 
	DOHWARS_CULLMODE                  = 22,	  // Culling State 
	DOHWARS_ALPHABLENDENABLE          = 27,   // TRUE to enable alpha blending 
	DOHWARS_FOGENABLE                 = 28,   // TRUE to enable fog blending  
	DOHWARS_SPECULARENABLE            = 29,   // TRUE to enable specular  
	DOHWARS_LIGHTING                  = 137,
	DOHWARS_AMBIENT                   = 139,

	//렌더링 상태값이 계속 추가될 예정.
	//..

	DOHWARS_MAX_
};



/////////////////////////////////////////////////////////////////////////////// 
//
// Fill Mode 별 옵션 : D3DFILLMODE  대응
//
//
enum DOHWAFILLMODE {
	DOHWAFILL_POINT = 1,		//점으로 그리기.
	DOHWAFILL_WIREFRAME = 2,		//선으로 그리기.
	DOHWAFILL_SOLID = 3,		//(지정)색으로 채우기.

	DOHWAFILL_FORCE_DWORD = 0x7fffffff,
};

/////////////////////////////////////////////////////////////////////////////// 
//
// Culling Mode 별 옵션.: D3DCULL 대응
//
enum DOHWACULL {
	DOHWACULL_NONE = 1,	//컬링 없음. 
	DOHWACULL_CW = 2,	//시계방향 컬링.
	DOHWACULL_CCW = 3,	//반시계방향 컬링.(기본값)

	DOHWACULL_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
};

/////////////////////////////////////////////////////////////////////
//
// DOHWA 정점 규격 정의 : D3DFVF 대응
//
#define  DOHWAFVF_XY			 0x0001		//!< 2D 좌표.(DX 미지원,  DOHWA 전용)★
#define  DOHWAFVF_XYZ			 0x0002		//!< 3D 좌표.(미 변환)(Local)
#define  DOHWAFVF_XYZRHW		 0x0004		//!< 3D 좌표.(변환 완료) XY + Depth
#define  DOHWAFVF_NORMAL		 0x0010		//!< 노멀 (Normal) 
#define  DOHWAFVF_DIFFUSE		 0x0040		//!< 확산색(Diffuse) 색상
#define  DOHWAFVF_SPECULAR		 0x0080		//!< 정반사(Specular) 색상. 
#define  DOHWAFVF_TEX1			 0x0100		//!< 텍스처 좌표.1개
#define  DOHWAFVF_TEX2			 0x0200		//!< 텍스처 좌표.2개

#define CHECK(value, bit) (((value) & (bit)) == bit)

/////////////////////////////////////////////////////////////////////
//
//	DOHWA 스왑체인 정보기술 구조체 : D3DPRESENT_PARAMETERS 대응  
//
struct DOHWAPRESENT_PARAMETERS {

	DWORD		Width;				//!< 해상도 : 클라이언트 영역기준.
	DWORD		Height;
	DWORD		BackBuffercnt;		//!< 백버퍼 개수
	BOOL		Windowed;			//!< 창모드 실행 실행 여부.  1 = Windowed, 0 = Full-Screen mode
};

//////////////////////////////////////////////////////////////////////
//
// DOHWA 출력 모드 정보 구조체 : D3DDISPLAYMODE 대응
//
struct DOHWADISPLAYMODE
{
	DWORD Width, Height;		//출력 어뎁터 : 화면 해상도 (ex) 800x600
};

////////////////////////////////////////////////////////////////////
//
// DOHWA 장치 생성 플래그 : 연산 가속화 정의, D3DCREATE 대응
//
enum DOHWACREATE
{
	DOHWACREATE_SOFTWARE_VERTEXPROCESSING,	//!< CPU 가 정점 연산을 처리.<Yena 기본>
	DOHWACREATE_HARDWARE_VERTEXPROCESSING,	//!< GPU 가 정점 연산을 처리.<Yena 불가>
	DOHWACREATE_MIXED_VERTEXPROCESSING,		//!< CPU / GPU 혼성 연산 가능.
	DOHWACREATE_MULTITHREADED				//!< 다중쓰레드 환경 호환성 추가.
};

///////////////////////////////////////////////////////////////////
//
// DOHWA 정점 버퍼 정보 기술 : D3DVERTEXBUFFER_DESC 대응
//
struct DOHWAVERTEXBUFFER_DESC
{
	DOHWAFORMAT           Format;		//!< 버퍼 포멧
	DOHWARESOURCETYPE     Type;			//!< 자원 형식
	DWORD				  Usage;		//!< 자원 용도
	DOHWAPOOL             Pool;			//!< 메모리 형식
	UINT                  Size;			//!< 크기 (Bytes)

	DWORD                 FVF;			//!< 정점 규격
};