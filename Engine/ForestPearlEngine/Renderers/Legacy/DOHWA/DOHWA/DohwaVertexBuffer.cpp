/////////////////////////////////////////////////////////////////////////////// 
//
// 시스템 / 플랫폼 헤더
//
#include "Dohwa.h"

///////////////////////////////////////////////////////////////////////////////
//
// Dohwa SWR 구현 클래스 선언 : 외부 노출 방지용.★
//
#include "DohwaClass.h"
#include "DohwaVertexBuffer.h"

/////////////////////////////////////////////////////////////////////////////// 
//
// class DohwaYenaVertexBuffer : 정점 버퍼.관리 클래스
//						    : IDirect3DVertexBuffer9 대응
//
/////////////////////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////////////////////
//
DohwaVertexBuffer9::DohwaVertexBuffer9(void)
{
	m_pVBuffer = NULL;
	m_SizeInByte = 0;
	m_FVF = 0;
	m_Stride = 0;
	m_VtxCnt = 0;
	m_bLocked = FALSE;

	ZeroMemory(&m_Desc, sizeof(m_Desc));
}

///////////////////////////////////////////////////////////////////////////////
//
DohwaVertexBuffer9::~DohwaVertexBuffer9(void)
{
	SafeDelArry(m_pVBuffer);
}

////////////////////////////////////////////////////////////////////////////////
//
// 사용자가 지정한 옵션으로 버퍼를 생성합니다.  
// <DX> 주어진 옵션에 맞추어 VRAM 을 확보합니다.   
// <Dohwa> 일반 시스템 메모리를 생성합니다.  
// 
// desc 생성할 정점버퍼 정보기술 구조체 
//
int DohwaVertexBuffer9::Create(DOHWAVERTEXBUFFER_DESC desc)
{
	//이미 만들어져 있다면 오류..사용자에게 해결을 요구. 
	//예외처리 추가를 추천합니다.
	if (DOHWA_VALIED(m_pVBuffer)) return DOHWA_FAIL;

	//정점 버퍼 메모리 확보. 
	//DX 처럼 VRAM 을 확보할 수 없으므로, Pool 옵션은 무시합니다. 
	m_pVBuffer = static_cast<void*>(new BYTE[desc.Size]);	assert(m_pVBuffer);
	m_SizeInByte = desc.Size;
	m_FVF = desc.FVF;
	m_Desc = desc;

	//정점 1개의 크기(바이트) 계산하기.
	//지정된 정점규격(FVF) 기준 계산, 가능한 단순하게 처리합니다.
	m_Stride = 0;
	if (CHECK(m_FVF, DOHWAFVF_XY))		m_Stride += sizeof(float) * 2;	//2d 좌표.(앞으로 사용하지 않겠지만, 사용 예로써 남겨둡니다) ★
	if (CHECK(m_FVF, DOHWAFVF_XYZ))		m_Stride += sizeof(float) * 3;	//3d 좌표.★
	if (CHECK(m_FVF, DOHWAFVF_DIFFUSE))	m_Stride += sizeof(DWORD);		//DWORD 형 색상.★

	//오류 체크..
	if (m_Stride <= 0) {  /* Error!!...  */  return DOHWA_FAIL; }

	//정점 개수 확인.(디버깅용)
	m_VtxCnt = GetVtxCnt();

	return DOHWA_OK;
}

////////////////////////////////////////////////////////////////////////////////
//
// 사용자가 지정한 옵션으로 버퍼를 생성합니다.  
// <DX> 주어진 옵션에 맞추어 VRAM 을 확보합니다.   
// <Dohwa> 일반 시스템 메모리를 생성합니다.  
// 
// Length	정점 버퍼 전체 크기 (바이트)	
// FVF		정점 규격. 
// Pool		메모리 풀(사용) 옵션.  	
//
int DohwaVertexBuffer9::Create(UINT Length, DWORD FVF, DOHWAPOOL Pool)
{
	// 생성할 정점 버퍼 정보 기술
	// <Dohwa> 시스템 메모리를 사용하므로 유의미한 동작은 없으나, 구조의 일관성을 위해 사용.
	// GPU 메모리(VRAM) 확보를 위해서 이 정도의 수고는 필요하다...정도로 이해해 봅시다.
	DOHWAVERTEXBUFFER_DESC desc = {};
	desc.Format = DOHWAFMT_VERTEXDATA;
	desc.Type = DOHWARTYPE_VERTEXBUFFER;
	desc.Usage = DOHWAUSAGE_WRITEONLY;			//정점버퍼는 CPU "쓰기전용", GPU "읽기 전용" 임을 상기합시다.
	desc.Pool = Pool;
	desc.Size = Length;
	desc.FVF = FVF;
	int res = Create(desc);

	return res;
}


////////////////////////////////////////////////////////////////////////////////
//
// 현재 정점 버퍼내의 정점 개수를 리턴합니다.
// 
// \return	 정점 개수
//
// 
UINT DohwaVertexBuffer9::GetVtxCnt()
{
	m_VtxCnt = m_SizeInByte / m_Stride;
	return m_VtxCnt;
}

////////////////////////////////////////////////////////////////////////////////
//
// GetPos2 : 정점 버퍼에서 좌표를 획득한다.(2D)
// 
// warning 소스의 간결함, 용이한 유지보수가 목적. 단, '약속된' 정점 규격 FVF 구조체의 
//			 데이터 열거 '순서'를 지키지 않으면 무용지물. 비정상 동작발생.★
// 
// index	정보를 획득할 정점 색인
// \return	정점 좌표 리턴
//
DOHWAVECTOR2 DohwaVertexBuffer9::GetPos2(int index)
{
	BYTE* pCurrBuffer = (BYTE*)m_pVBuffer + (index)*m_Stride;

	DOHWAVECTOR2  pos2 = *(DOHWAVECTOR2*)pCurrBuffer;

	return pos2;
}

////////////////////////////////////////////////////////////////////////////////
//
// GetPos3 : 정점 버퍼에서 좌표를 획득한다.(3D)
// 
// warning 소스의 간결함, 용이한 유지보수가 목적. 단, '약속된' 정점 규격 FVF 구조체의 
//			 데이터 열거 '순서'를 지키지 않으면 무용지물. 비정상 동작발생.★
// 
// index	정보를 획득할 정점 색인
// return	정점 좌표 리턴
//
DOHWAVECTOR3 DohwaVertexBuffer9::GetPos3(int index)
{
	BYTE* pCurrBuffer = (BYTE*)m_pVBuffer + (index)*m_Stride;

	DOHWAVECTOR3  pos3 = *(DOHWAVECTOR3*)pCurrBuffer;

	return pos3;
}

////////////////////////////////////////////////////////////////////////////////
//
// GetDiffuse : 정점 버퍼에서 정점색을 획득한다.
// 
// remarks (현재 원본) 정점 버퍼에는 DWORD 형 색상이 저장되어 있음. 
// 
// warning 소스의 간결함, 용이한 유지보수가 목적. 단, '약속된' 정점 규격 FVF 구조체의 
//			 데이터 열거 '순서'를 지키지 않으면 무용지물. 비정상 동작발생.★
// 
// index	정보를 획득할 정점 색인
// return	정점 색상 리턴 (DWORD 형)
//
DWORD DohwaVertexBuffer9::GetDiffuse(int index)
{
	BYTE* pCurrBuffer = (BYTE*)m_pVBuffer + (index)*m_Stride;

	//(현재 원본) 정점 버퍼에는 DWORD 형 색상이 저장되어 있음. 
	DWORD color = 0;

	//-----------------------------------------------
	// 현재 장치에 설정된 FVF 별 구성 확인 후 색상 획득.★
	// <주1> 개선이 필요하나 수업 진도를 위해 생략...
	// <주2> '약속된' 정점 규격-구조체의 데이터 열거 '순서'를 사용자가 지키지 않으면 무용지물. 비정상 동작발생.★
	//-----------------------------------------------
	if (CHECK(m_FVF, DOHWAFVF_XY))								//2D 는 교과내에서 더이상 사용할 경우가 없겠으나
		color = *(DWORD*)(pCurrBuffer + sizeof(DOHWAVECTOR2));	//다양한 FVF 구성 별 정점 데이터 읽기 방식의 예로 남겨둠.

	if (CHECK(m_FVF, DOHWAFVF_XYZ))								//정점의 FVF 별로 확인이 필요 ★ 	 
		color = *(DWORD*)(pCurrBuffer + sizeof(DOHWAVECTOR3));	//클래스 자료형은 vtable 등의 이유로 크기가 다름, 주의.★	 

	return color;
}

////////////////////////////////////////////////////////////////////////////////
//
// 정점 버퍼 잠그기.  
// 
// <DX> 정점 버퍼(VRAM) 에 데이터를 입력하기 위해서는 CPU 가 접근 가능해야 합니다.  
// Lock 함수는 버퍼를 CPU 의 "쓰기 또는 읽기" 가능 상태로 변경합니다.  
// 전체 버퍼 및 부분 잠금이 가능하며 잠금된 영역은 GPU 의 접근이 제한됩니다.("접근 잠금")  
// 사용이 끝난 버퍼는 UnLock 함수로 잠금을 해제해야 합니다.  
// 
// <Dohwa> "잠금" 및 "해제" 동작을 SW 로 구현,묘사 가능하겠으나
// 시스템 메모리를 사용중으로 특별한 기능은 추가하지 않겠습니다.  
// DX 의 실제 역활과는 다르지만, 동작의 흐름은 유지합니다.
// 
// OffsetToLock	'잠금' 할 버퍼 (옵셋) 주소. 기본값 0,
// SizeToLock		'잠금' 할 버퍼 크기
// ppbData			리턴받을 버퍼 포인터
// Flags			'잠금' 옵션. 기본값 0.	
//
int DohwaVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
	if (DOHWA_ENABLED(m_bLocked))							//잠금 상태 확인...
	{
		return DOHWA_FAIL;
	}

	if (DOHWA_VALIED(*ppbData)) return DOHWA_FAIL;			//유효성 검사

	BYTE* pVB = (BYTE*)m_pVBuffer + OffsetToLock;		//지정 주소 계산.
	*ppbData = (void*)pVB;								//주소 넘김. 

	//잠금 상태 설정.
	m_bLocked = TRUE;

	return DOHWA_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
// Unlock 정점 버퍼 잠금 해재  
// 
// <DX> 현재 버퍼의 잠금 상태를 해제 합니다. 
// 이후 버퍼는 원 상태 - CPU "쓰기 불가", GPU "읽기/접근 가능" 상태가 되며
// 파이프라인에서 사용가능해 집니다. 
// Lock 과 UnLock 함수는 쌍(Pair) 로 사용되어야 합니다.  
// 
// <Dohwa> "잠금" 및 "해제" 동작을 SW 로 구현,묘사 가능하겠으나
// 시스템 메모리를 사용중으로 특별한 기능은 추가하지 않겠습니다.
// 
//
int DohwaVertexBuffer9::Unlock(void)
{
	if (DOHWA_DISABLED(m_bLocked))							//잠금 상태 확인...
	{
		return DOHWA_FAIL;	
	}

	//<Dohwa> "잠금" 및 "해제" 동작을 SW 로 구현,묘사 가능하겠으나
	//시스템 메모리를 사용중으로 특별한 기능은 추가하지 않겠습니다.
	//...

	// 현재 버퍼의 정점개수 정도 확인만 수행.★
	m_VtxCnt = m_SizeInByte / m_Stride;

	//잠금 상태 설정.
	m_bLocked = FALSE;

	return DOHWA_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
// GetDesc 정점 버퍼 정보 획득   
// 
// [out]	pDesc	정점 버퍼 정보기술 구조체
//
int DohwaVertexBuffer9::GetDesc(_out_ DOHWAVERTEXBUFFER_DESC* pDesc)
{
	if (DOHWA_INVALIED(pDesc)) return DOHWA_FAIL;			//유효성 검사.

	*pDesc = m_Desc;

	return DOHWA_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
// GetBuffer : 버퍼 획득 <Dohwa> 전용  
// 
// return	정점 버퍼 포인터
//
void* DohwaVertexBuffer9::GetBuffer()
{
	if (DOHWA_INVALIED(m_pVBuffer))
	{
		return NULL;
	}

	return m_pVBuffer;
}




////////////////////////////////////////////////////////////////////////////////
//
// GetVertexCount : 정점 개수 획득 <Dohwa> 전용
// 
// return	정점 개수
//
UINT DohwaVertexBuffer9::GetVertexCount()
{
	if (DOHWA_INVALIED(m_pVBuffer))
	{
		return 0;
	}

	if (m_Desc.Size <= 0 || m_Stride == 0)
	{
		return 0;
	}

	m_VtxCnt = m_Desc.Size / m_Stride;
	return m_VtxCnt;
}




////////////////////////////////////////////////////////////////////////////////
//
// GetPrivateData : 버퍼 정보 획득 <Yena> 용 재구성
// 
// [out]	ppBuffer	정점 버퍼 포인터
// [out]	pSizeData	정점 개수
//
int DohwaVertexBuffer9::GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData)
{
	void* pVB = GetBuffer();
	UINT  vtxs = GetVertexCount();

	//외부로 리턴.
	*ppBuffer = pVB;
	*pSizeData = vtxs;

	return DOHWA_OK;
}




////////////////////////////////////////////////////////////////////////////////
//
// AddRef : Dohwa COM 참조 카운트 증가 
// 
// return	현재 참조 카운트
//
ULONG DohwaVertexBuffer9::AddRef(void)
{
	return ++m_ref;
};




////////////////////////////////////////////////////////////////////////////////
//
// Release : Dohwa COM 참조 카운트 감소, 0 이하면 객체 제거.
// 
// return	현재 참조 카운트
//
ULONG DohwaVertexBuffer9::Release(void)
{
	if (--m_ref <= 0) delete this;

	return m_ref;
};




////////////////////////////////////////////////////////////////////////////////
//
// QueryInterface : Dohwa COM 인터페이스 획득.  
// 
// <COM> 객체에 인터페이스를 질의하고 유효한 경우 해당 인터페이스가 리턴됩니다.  
//	지정 인터페이스 ID 로 참조되는 (원본) 객체는 참조 카운트가 증가되므로 
// 인터페이스 사용 후 안전한 메모리 해제를 위해 Release 를 호출해야 합니다.  
// 
// <Dohwa> Dohwa 전용 GUID 를 통해 해당 ynInterface 를 획득하며 
// 기본적인 기능은 Windows / COM 과 동일합니다.  
// 
// riid		Yena 인터페이스 ID
// ppvObject	(생성된) Dohwa 인터페이스 포인터.
//
int DohwaVertexBuffer9::QueryInterface(DH_IID riid, _out_ void** ppvObject)
{
	//void* pInterface = nullptr;

	//지정 ID 의 유효성 검사..(간략)

	if (DHIsEqualIID(riid, IID_IDohwaVertexBuffer9))		//IID 동일 검사 <상동>
	{
		AddRef();												//참조 카운트 증가.
		*ppvObject = dynamic_cast<IDohwaVertexBuffer9*>(this);	//지정 인터페이스 리턴.
	}

	return DOHWA_OK;
}