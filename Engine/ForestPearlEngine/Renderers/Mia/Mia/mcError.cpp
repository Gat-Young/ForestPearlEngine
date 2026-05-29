#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
	#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#include "Mia.h"						//Yena 엔진 필수 헤더.
#include "mcError.h"

namespace MIA
{
	MIA_APIX int mcErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, mcException& e, TCHAR* msg, ...)
	{
		TCHAR msgva[1024] = _T("");
		va_list vl;
		va_start(vl, msg);
		_vstprintf(msgva, msg, vl);
		va_end(vl);

		return mcErrorW(file, line, func, bMBox, e.hr(), msgva);
	}

	MIA_APIX int mcErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
	{
		TCHAR msgva[1024] = _T("");
		va_list vl;
		va_start(vl, msg);
		_vstprintf(msgva, msg, vl);
		va_end(vl);

		//사용자 메세지 출력.
		TCHAR usermsg[1024] = _T("");
		{
			_stprintf(usermsg, _T("\n> [Mia] %s \t Func = %s : File = %s (%d) \n"), msgva, func, file, line);
			OutputDebugString(usermsg);
		}

		//Mia COM/DX 에러 메세지 출력.
		TCHAR outmsg[1024] = _T("");
		TCHAR hrmsg[1024] = _T("");
		if (SUCCEEDED(hr))
		{
			//성공 상태, 메세지 간단 출력...
		}
		else
		{
			//오류 있음, 에러 메세지 (상세) 출력
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
				0, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), hrmsg, 1024, NULL);
			_stprintf(outmsg, _T("> [Mia] 에러코드(0x%X) : hr=%s \n"), hr, hrmsg);
			OutputDebugString(outmsg);
			_stprintf(outmsg, _T(">\t\t Func = %s \n>\t\t File = %s (%d) \n\n"), func, file, line);
			OutputDebugString(outmsg);
		}

		if (bMBox)
		{
			TCHAR msg[2048] = _T("");
			_stprintf(msg, _T("%s \n[Mia] 에러코드(0x%X) : hr=%s \nFunc = %s \nFile = %s (%d)"),
				msgva, hr, hrmsg, func, file, line);
			MessageBox(NULL, msg, _T("Mia::Error"), MB_OK | MB_ICONERROR);
		}
		return MC_OK;
	}

	MIA_APIX std::string  mcStrFmtVA(const CHAR* fmt, ...)
	{
		char buff[1024] = "";
		va_list vl;
		va_start(vl, fmt);
		vsprintf(buff, fmt, vl);
		va_end(vl);
		return std::string(buff);
	}

	MIA_APIX std::wstring mcStrFmtVW(const WCHAR* fmt, ...)
	{
		WCHAR buff[1024] = L"";
		va_list vl;
		va_start(vl, fmt);
		vswprintf(buff, fmt, vl);
		va_end(vl);
		return std::wstring(buff);
	}

	MIA_APIX std::basic_string<TCHAR>  mcStrFmtVT(const TCHAR* fmt, ...)
	{
		TCHAR buff[1024] = _T("");
		va_list vl;
		va_start(vl, fmt);
		_vstprintf(buff, fmt, vl);
		va_end(vl);
		return std::basic_string<TCHAR>(buff);
	}
}