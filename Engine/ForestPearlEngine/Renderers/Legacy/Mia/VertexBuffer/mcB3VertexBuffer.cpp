#include "../Mia.h"

#include "../Define/mcB3Class.h"
#include "mcB3VertexBuffer.h"

B3MiaVertexBuffer9::B3MiaVertexBuffer9(void)
{
	m_pVBuffer = NULL;
	m_SizeInByte = 0;
	m_FVF = 0;
	m_Stride = 0;
	m_VtxCnt = 0;
	m_bLocked = FALSE;

	ZeroMemory(&m_Desc, sizeof(m_Desc));

#ifdef _DEBUG
	MIA::mcLog(_T("B3MiaVertexBuffer9 생성됨..."));
#endif
}


B3MiaVertexBuffer9::~B3MiaVertexBuffer9(void)
{
	SafeDelArray(m_pVBuffer);

#ifdef _DEBUG
	MIA::mcLog(_T("B3MiaVertexBuffer9 제거됨..."));
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
	if (CHECK(m_FVF, B3MFVF_XY))		m_Stride += sizeof(float) * 2;
	if (CHECK(m_FVF, B3MFVF_XYZ))		m_Stride += sizeof(float) * 3;
	if (CHECK(m_FVF, B3MFVF_DIFFUSE))	m_Stride += sizeof(DWORD);

	if (m_Stride <= 0) { return MC_FAIL; }

	m_VtxCnt = _GetVtxCnt();

	return MC_OK;
}


UINT B3MiaVertexBuffer9::_GetVtxCnt()
{
	m_VtxCnt = m_SizeInByte / m_Stride;
	return m_VtxCnt;
}


B3MVECTOR2 B3MiaVertexBuffer9::_GetPos2(int index)
{
	BYTE* pCurrBuffer = (BYTE*)m_pVBuffer + (index)*m_Stride;

	B3MVECTOR2  pos2 = *(B3MVECTOR2*)pCurrBuffer;

	return pos2;
}


B3MVECTOR3 B3MiaVertexBuffer9::_GetPos3(int index)
{
	BYTE* pCurrBuffer = (BYTE*)m_pVBuffer + (index)*m_Stride;

	B3MVECTOR3  pos3 = *(B3MVECTOR3*)pCurrBuffer;

	return pos3;
}


DWORD B3MiaVertexBuffer9::_GetDiffuse(int index)
{
	BYTE* pCurrBuffer = (BYTE*)m_pVBuffer + (index)*m_Stride;

	DWORD color = 0;

	if (CHECK(m_FVF, B3MFVF_XY))
		color = *(DWORD*)(pCurrBuffer + sizeof(B3MVECTOR2));

	if (CHECK(m_FVF, B3MFVF_XYZ))
		color = *(DWORD*)(pCurrBuffer + sizeof(B3MVECTOR3));

	return color;
}


int B3MiaVertexBuffer9::Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
{
	if (MC_ENABLED(m_bLocked))
	{
		MIA::mcLog(_T("이 버퍼는 이미 잠겨(Locked) 있습니다. %s=%x)"), mcToString(m_pVBuffer), m_pVBuffer);
	}

	if (MC_VALIED(*ppbData))
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
		MIA::mcLog(_T("이 버퍼는 이미 잠금 해제(Unlocked) 상태 입니다. %s=%x)"), mcToString(m_pVBuffer), m_pVBuffer);
	}

	m_VtxCnt = m_SizeInByte / m_Stride;

	m_bLocked = FALSE;

	return MC_OK;
}


int B3MiaVertexBuffer9::GetDesc(_out_ B3MVERTEXBUFFER_DESC* pDesc)
{
	if (MC_INVALIED(pDesc)) return MC_FAIL;

	*pDesc = m_Desc;

	return MC_OK;
}


void* B3MiaVertexBuffer9::GetBuffer()
{
	if (MC_INVALIED(m_pVBuffer))
	{
		MIA::mcLog(_T(" NULL 객체를 참조하고 있습니다 : %s = NULL, Size=%d, Stride=%d"),
			mcToString(m_pVBuffer), m_Desc.Size, m_Stride);
	}

	return m_pVBuffer;
}


UINT B3MiaVertexBuffer9::GetVertexCount()
{
	if (MC_INVALIED(m_pVBuffer))
	{
		MIA::mcLog(_T(" NULL 객체를 참조하고 있습니다 : %s = NULL, Size=%d, Stride=%d"),
			mcToString(m_pVBuffer), m_Desc.Size, m_Stride);
	}

	if (m_Desc.Size <= 0 || m_Stride == 0)
	{
		MIA::mcLog(_T(" 미설정 객체를 참조하고 있습니다 : %s = %x , Size=%d, Stride=%d"),
			mcToString(m_pVBuffer), m_pVBuffer, m_Desc.Size, m_Stride);
	}

	m_VtxCnt = m_Desc.Size / m_Stride;
	return m_VtxCnt;
}


int B3MiaVertexBuffer9::GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData)
{
	void* pVB = GetBuffer();
	UINT  vtxs = GetVertexCount();

	*ppBuffer = pVB;
	*pSizeData = vtxs;

	return MC_OK;
}


ULONG B3MiaVertexBuffer9::AddRef(void)
{
	return ++m_ref;
};


ULONG B3MiaVertexBuffer9::Release(void)
{
	if (--m_ref <= 0) delete this;

	return m_ref;
};


int B3MiaVertexBuffer9::QueryInterface(MC_IID riid, _out_ void** ppvObject)
{
	if (mcIsEqualIID(riid, IID_IMiaVertexBuffer9))
	{
		AddRef();
		*ppvObject = dynamic_cast<IMiaVertexBuffer9*>(this);
	}

	return MC_OK;
}