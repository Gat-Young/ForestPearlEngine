#pragma once

#include "windows.h"

/////////////////////////////////////////////////////////////
//
// B3Mia 버퍼 규격 : D3DFORMAT 대응
//
enum B3MFORMAT
{
	B3MFMT_UNKNOWN = 0,
	B3MFMT_A8R8G8B8 = 21,
	B3MFMT_D32 = 71,
	B3MFMT_VERTEXDATA = 100,
	B3MFMT_INDEX16 = 101,
	B3MFMT_INDEX32 = 102,
};

/////////////////////////////////////////////////////////////
//
// B3Mia 자원 규격 : D3DRESOURCETYPE 대응
//
enum B3MRESOURCETYPE {
	B3MRTYPE_SURFACE = 1,       //일반 "서피스" 버퍼
	B3MRTYPE_TEXTURE = 3,       //텍스처
	B3MRTYPE_VERTEXBUFFER = 6,  //정점 버퍼
	B3MRTYPE_INDEXBUFFER = 7,   //색인 버퍼
};

/////////////////////////////////////////////////////////////
//
// B3Mia 메모리 풀(Memory Pool) 종류 : D3DPOOL 대응
//
enum B3MPOOL
{
	B3MPOOL_SYSTEMMEM = 2,  //시스템 메모리 사용
};

//typedef B3MPOOL	B3MIAPOOL;			//구형 호환성 유지

/////////////////////////////////////////////////////////////
//
// B3Mia 버퍼 용도 정의 : D3DUSAGE_XXX 대응
//
////////////////////////////////
// Usages for RT, DS.
#define B3MUSAGE_RENDERTARGET  (0x00000001L)
#define B3MUSAGE_DEPTHSTENCIL  (0x00000002L)
#define B3MUSAGE_DYNAMIC       (0x00000200L)

////////////////////////////////
// Usages for Vertex/Index buffers
#define B3MUSAGE_WRITEONLY          (0x00000008L)
#define B3MUSAGE_SOFTWAREPROCESSING (0x00000010L)

/////////////////////////////////////////////////////////////
//
// 기하도형 타입 : D3DPRIMITIVETYPE 대응
//
enum B3MPRIMITIVETYPE
{
	B3MPT_LINELIST = 2,
	B3MPT_TRIANGLELIST = 4,
};

/////////////////////////////////////////////////////////////
//
// 정점 규격 정의 (하나 이상의 규격 조합 가능) : D3DFVF 대응
//
#define  B3MFVF_XY	    0x0001 //2D 좌표. (DX 미지원)
#define  B3MFVF_XYZ	    0x0002 //3D 좌표. (미 변환)
#define  B3MFVF_DIFFUSE 0x0040		//확산색(Diffuse) 색상

#define CHECK( value, bit) (((value) & (bit)) == bit)


/////////////////////////////////////////////////////////////
//
// 렌더링 상태 옵션 : D3DRENDERSTATETYPE 대응
//
enum B3MRENDERSTATETYPE {
	B3MRS_FILLMODE = 8,
	B3MRS_CULLMODE = 22,
	B3MRS_MAX_
};
////////////////////////////////
// 모드 별 옵션
enum B3MFILLMODE {
	B3MFILL_POINT = 1,
	B3MFILL_WIREFRAME = 2,
	B3MFILL_SOLID = 3,
};
enum B3MCULL {
	B3MCULL_NONE = 1,
	B3MCULL_CW = 2,
	B3MCULL_CCW = 3,
};

/////////////////////////////////////////////////////////////
//
// 렌더타겟 (백버퍼) - 정보 설정 구조체 : D3DPRESENT_PARAMETERS 대응
//
// Mia.h에서 가져옴
// 이름 개정
struct B3MPRESENT_PARAMETERS
{
	DWORD Width;
	DWORD Height;
	DWORD BackBuffercnt;
	BOOL  Windowed;
};

typedef B3MPRESENT_PARAMETERS	MIAPRESENT_PARAMETERS;	// 이름 개정
typedef B3MPRESENT_PARAMETERS	MCPRESENT_PARAM;


/////////////////////////////////////////////////////////////
//
// 해상도 정보 구조체. : D3DDISPLAYMODE 대응
//
// Mia.h에서 가져옴
// 이름 개정
struct B3MDISPLAYMODE
{
	DWORD Width, Height;
};

typedef B3MDISPLAYMODE MIADISPLAYMODE;	// 이름 개정

/////////////////////////////////////////////////////////////
//
// 장치 생성 플래그 : 연산 가속화 정의, D3DCREATE 대
//
// 이름 개정
enum B3MCREATE
{
	MIACREATE_SOFTWARE_VERTEXPROCESSING,	//!< CPU 가 정점 연산을 처리.
	MIACREATE_HARDWARE_VERTEXPROCESSING,	//!< GPU 가 정점 연산을 처리.
};

typedef B3MCREATE MIACREATE;                  // 이름 개정
//typedef MIACREATE	BEHEVIOR_FLAG;			//구형 호환성 유지

/////////////////////////////////////////////////////////////
//
// Vertex Buffer Description : 정점 버퍼 정보 기술, D3DVERTEXBUFFER_DESC 대응
//
struct B3MVERTEXBUFFER_DESC
{
	B3MFORMAT        Format;	// 버퍼 포멧
	B3MRESOURCETYPE  Type;		// 자원 형식
	DWORD            Usage;		// 자원 용도
	B3MPOOL          Pool;		// 메모리 형식
	UINT             Size;		// 크기(Bytes)

	DWORD            FVF;		// 정점 규격
};