#pragma once

///////////////////////////////////////////////////////////////
//
// 시스템 / 플랫폼 헤더
//
#include <Windows.h>
#pragma warning(disable : 4305)

/////////////////////////////////////////////////////////////////////////////
//
#define YES_ 1
#define NO_	 0

/////////////////////////////////////////////////////////////////////////////
//
// 메소드/함수 팔라미터 속성 표시용 (가독성 향상)
//
#ifndef _in_
#define _in_            //input
#define _in_out_        //input & output
#define _inout_         //input & output
#define _out_           //output
#define _out_opt_       //output, option
#define _opt_           //option
#define _in_opt_        //input, option
#endif

//폐기 경고 : C4996 에러 적용됨.
#ifndef _deprecated_
#define _not_implemented_	[[deprecated("> 미구현 - 사용 중지")]]
#define _deprecated_		[[deprecated("> 폐기됨 - 사용 중지")]]
#define DOHWA_NOT_IMPLEMENTED	_not_implemented_;
#define DOHWA_미구현_미사용		//! \brief \emoji :construction: \emoji :no_entry: 미구현, 미사용
#define DOHWA_폐기됨_미사용		//! \brief \emoji :construction: \emoji :no_entry: 폐기됨, 사용중지
#define DOHWA_DEPRECATED		_deprecated_ 
#define DOHWA_DEPRECATED_(s)	DOHWA_폐기됨_미사용##(s)
#define DohwaDeprecatedMsg(s)	DohwaBuildMsg("폐기됨 - 사용중지 "s)
#endif

#ifndef _omitted_
#define _omitted_
#define _기능_생략_		//_omitted_
#define DOHWA_기능_생략_	//_omitted_
#endif

/////////////////////////////////////////////////////////////////////////////
//
#include "assert.h"
#define ASSERT(Val) assert((Val))

#define DOHWA_INVALIED(res) ((res) == NULL)
#define DOHWA_VALIED(res)	 ((res) != NULL)
#define DOHWA_FAILED(res)	 ((res) < 0 )
#define DOHWA_SUCCEEDED(res) ((res) == YN_OK )


#define DOHWA_ENABLED(res)	 ((res) == TRUE )
#define DOHWA_DISABLED(res) ((res) == FALSE )

#define DOHWA_OK		0 
#define DOHWA_FALSE		-1 
#define DOHWA_FAIL		DOHWA_FALSE
#define DOHWA_NULL		nullptr

//렌더링 상태/오류
#define DOHWA_CULLED	0x80000010
#define DOHWA_CLIPPED	0x80000020
#define DOHWA_CHECK(res, v) (((res) & (v)) == (v))

/////////////////////////////////////////////////////////////////////////////
//
#define _LOGFILE_ON_			//로그 파일 생성하기..

// DXUtil.h 의 것을 사용함
//
#ifndef SafeDelete
#define SafeDelete(pBuff)	if((pBuff)){ delete (pBuff);	 (pBuff) = NULL; }
#define SafeDelArry(pBuff)	if((pBuff)){ delete [] (pBuff);  (pBuff) = NULL; }
#define SafeRelease(pBuff)	if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; } //★
#endif
