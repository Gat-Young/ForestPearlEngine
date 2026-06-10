#pragma once

#ifdef MIA_DLL
	#define MIA_API	extern "C" __declspec(dllexport)
	#define MIA_APIX	__declspec(dllexport)
#else
	#define MIA_API 
	#define MIA_APIX 
#endif


namespace MIA
{
	// 에러 메세지 처리
	MIA_APIX int mcErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);

	// 에러 메세지 출력 : 일반+상세+MB(ON)
#define mcError(hr, msg, ...) mcErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )

	// 에러 메세지 출력 : 디버그+간단+MB(OFF), VS Output 간단 로그 출력용
#define mcLog(msg, ...)	mcErrorW( __FILEW__, __LINE__, __FUNCTIONW__, FALSE, S_OK, msg, __VA_ARGS__ )

	// 문자열 전환
#ifndef mcToString
#define mcToString(v)  _T(#v)		// 문자열 전환.
#define ToString(v)    _T(#v)		// 문자열 전환 : 구형 호환성 유지용. 
#endif

	// 가변인자 문자열 구성 함수.
	MIA_APIX std::string  mcStrFmtVA(const CHAR* fmt, ...);
	MIA_APIX std::wstring mcStrFmtVW(const WCHAR* fmt, ...);
	MIA_APIX std::basic_string<TCHAR>  mcStrFmtVT(const TCHAR* fmt, ...);

#ifdef _UNICODE
#define mcStrFmtV(fmt, ...)	mcStrFmtVW(fmt, __VA_ARGS__)
#else
#define mcStrFmtV(fmt, ...)	mcStrFmtVA(fmt, __VA_ARGS__)
#endif


	/////////////////////////////////////////////////////////////
	//
	// 에러처리 클래스
	//
#include <exception>

	class mcException : public std::exception
	{
	public:
		mcException() = delete;
		mcException(HRESULT hr) : m_hr(hr) { getHRMessage(); }
		virtual ~mcException() = default;

	private:
		HRESULT m_hr;
		TCHAR m_strhr[1024] = _T("");
		TCHAR m_hrMsg[2048] = _T("");

	public:
		_NODISCARD				HRESULT hr() noexcept { return m_hr; }
		_NODISCARD virtual const TCHAR* what() noexcept { return m_hrMsg; }

		const TCHAR* getHRMessage() noexcept
		{
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK,
				0, m_hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				m_strhr, 1024, NULL
			);
			_stprintf_s(m_hrMsg, _T("\n> [Mia][mcException] Failure with HRESULT of 0x%08X : %s"),
				static_cast<unsigned int>(m_hr), m_strhr);
			return m_hrMsg;
		}
	};

	inline void mcThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw mcException(hr);
		}
	}

#define mcCheck			mcThrowIfFailed
#define mcCheckFail		mcThrowIfFailed
#define mcCheckError	mcThrowIfFailed

	MIA_APIX int mcErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, mcException& e, TCHAR* msg, ...);

#define mcError(e, msg, ...) mcErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, e, msg, __VA_ARGS__ )
}