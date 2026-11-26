#include "FPSystemTimer.h"
#include <iostream>

// 호출 시점의 시간을 기록하고 순차 증가하는 프로파일 ID를 발급
unsigned int FPSystemTimer::StartProfileTimer()
{
	unsigned int ProfileID = NextProfileID++;
	ProfileTimerMap[ProfileID] = std::chrono::steady_clock::now();

	return ProfileID;
}

// ProfileID와 기록된 시간 사이의 경과 시간을 LogText 뒤에 붙여 ms 단위로 출력하고 ID를 제거
void FPSystemTimer::EndProfileTimer(std::string LogText, unsigned int ProfileID)
{
	auto Iter = ProfileTimerMap.find(ProfileID);
	if (Iter == ProfileTimerMap.end())
		return;

	std::chrono::steady_clock::time_point StartTime = Iter->second;
	std::chrono::steady_clock::time_point EndTime = std::chrono::steady_clock::now();

	double ElapsedMs = std::chrono::duration<double, std::milli>(EndTime - StartTime).count();

	std::cout << LogText << ElapsedMs << "ms" << std::endl;

	ProfileTimerMap.erase(Iter);
}
