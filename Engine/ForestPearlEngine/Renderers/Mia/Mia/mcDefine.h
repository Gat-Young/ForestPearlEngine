#pragma once

#include "windows.h"


#define YES_ 1
#define NO_	 0

////////////////////////////////
// 메소드/함수 팔라미터 속성 표시용
#ifndef _in_
	#define _in_            //input
	#define _in_out_        //input & output
	#define _out_           //output
	#define _out_opt_       //output, option
	#define _opt_           //option
	#define _in_opt_        //input, option
#endif

#include "assert.h"
#define ASSERT(Val) assert((Val))

#define MC_INVALIED(res) ((res) == NULL)
#define MC_VALIED(res) ((res) != NULL)
#define MC_FAILED(res) ((res) < 0)
#define MC_SUCCEEDED(res) ((res) == MC_OK )

#define MC_ENABLED(res)	 ((res) == TRUE )
#define MC_DISABLED(res) ((res) == FALSE )

#ifndef MC_OK
	#define MC_OK	0
	#define MC_FALSE	-1
	#define MC_FAIL	MC_FALSE
	#define MC_NULL nullptr
#endif

#define _LOGFILE_ON_  //로그 파일 생성하기

#ifndef IsKeyDown
	#define IsKeyDown(k) ((GetAsyncKeyState(k) & 0x8000) == 0x8000)
	#define IsKeyUp(k) ((GetAsyncKeyState(k) & 0x8001) == 0x8001)
#endif

#ifndef SafeDelete
	//#define SafeRelease SafeDelete //(p) if((p)){ (p)->Release(); (p) = NULL; }
	#define SafeDelete(pBuff) if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
	#define SafeDelArray(pBuff) if((pBuff)){ delete[] (pBuff); (pBuff) = NULL; }
	#define SafeRelease(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#endif



//구형 호환성 유지
//#define B3M_PI	MC_PI
//#define MAKERADIAN	mcConvertToRadian