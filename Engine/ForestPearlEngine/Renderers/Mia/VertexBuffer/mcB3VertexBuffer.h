#pragma once

#include "windows.h"
#include "tchar.h"
#include "math.h"
#include "stdio.h"

#include "string"
#include "vector"
#include "algorithm"
using namespace std;

class B3MiaVertexBuffer9 : public IMiaVertexBuffer9
{
	friend class B3MiaDevice9;
	friend class B3MiaGraphicsEngine9;


protected:
	ULONG m_ref = 0;

protected:
	void* m_pVBuffer;
	DWORD m_SizeInByte;
	DWORD m_FVF;
	UINT  m_Stride;
	UINT  m_VtxCnt;
	BOOL  m_bLocked;

	B3MVERTEXBUFFER_DESC m_Desc;

protected:
	int		_Create(B3MVERTEXBUFFER_DESC desc);

	void* _GetVBuffer() { return m_pVBuffer; }
	DWORD	_GetSizeInByte() { return m_SizeInByte; }
	DWORD	_GetFVF() { return m_FVF; }
	DWORD	_GetStride() { return m_Stride; }
	UINT	_GetVtxCnt();

	B3MVECTOR2	_GetPos2(int index);
	B3MVECTOR3	_GetPos3(int index);
	DWORD		_GetDiffuse(int index);


public:
	B3MiaVertexBuffer9(void);
	virtual ~B3MiaVertexBuffer9(void);

	virtual int   Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
	virtual int   Unlock(void);
	virtual int   GetDesc(_out_ B3MVERTEXBUFFER_DESC* pDesc);

	virtual void* GetBuffer(void);
	virtual UINT  GetVertexCount(void);
	virtual int	  GetPrivateData(_out_ void** ppBuffer, _out_ UINT* pSizeData);

	virtual ULONG AddRef(void);
	virtual ULONG Release(void);
	virtual int   QueryInterface(MC_IID riid, _out_ void** ppvObject);

};

typedef B3MiaVertexBuffer9* LPB3MIAVERTEXBUFFER9;