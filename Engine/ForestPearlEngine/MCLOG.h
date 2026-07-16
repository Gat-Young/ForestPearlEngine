#pragma once
#include <iostream>
#include <vector>
#include <cstdio>

/////////////////////////////////////////////////////////////
//
//	MCLOG 사용법!
//
//	#include "LogMC.h"
//
//	using namespace MIA;
//
//	MCLOG("로그 태그- LogMC나 ErrorMC나 WarningMC 사용 가능", "로그메시지");
//  e.g., MCLOG(LogMC, "함수 호출됨 %d", 3);
// 
//  로그메시지를 생략하고 ""을 파라미터로 입력하면 "Called"가 자동으로 출력됩니다.
//
/////////////////////////////////////////////////////////////

#define LogMC "Log"
#define ErrorMC "Error"
#define WarningMC "Warning"

#define MCLOG(LogTag, Fmt, ...) PrintMCLog(__FUNCSIG__, LogTag, FormatMCLog(Fmt, ##__VA_ARGS__))

template<typename... Args>
inline std::string FormatMCLog(const std::string& fmt, Args&&... args)
{
	int size = std::snprintf(nullptr, 0, fmt.c_str(), args...) + 1;
	std::vector<char> buf(size);
	std::snprintf(buf.data(), size, fmt.c_str(), args...);
	return std::string(buf.data(), buf.data() + size - 1);
}

inline void PrintMCLog(const std::string& FunctionSig = "", const std::string& LogState = "Log", const std::string& LogMessage = "Called.")
{
	std::string ClassName;
	std::string FunctionName;
	// 클래스명 추출
	size_t parenPos = FunctionSig.find('(');
	if (parenPos == std::string::npos)
	{
		ClassName = "";
		FunctionName = "";
	}
	else
	{
		size_t lastColon = FunctionSig.rfind("::", parenPos);
		if (lastColon == std::string::npos)
		{
			// 전역 함수
			ClassName = "(Global)";
			size_t nameStart = FunctionSig.rfind(' ', parenPos) + 1;
			FunctionName = FunctionSig.substr(nameStart, parenPos - nameStart);
		}
		else
		{
			FunctionName = FunctionSig.substr(lastColon + 2, parenPos - lastColon - 2);
			size_t prevColon = FunctionSig.rfind("::", lastColon - 1);
			size_t start = (prevColon == std::string::npos) ? FunctionSig.rfind(' ', lastColon - 1) + 1 : prevColon + 2;
			ClassName = FunctionSig.substr(start, lastColon - start);
		}
	}
	if (LogMessage == "")
	{
		std::cout << LogState << ": [" << ClassName << "::" << FunctionName << "] " << "Called\n";
	}
	else
	{
		std::cout << LogState << ": [" << ClassName << "::" << FunctionName << "] " << LogMessage << "\n";
	}
}




