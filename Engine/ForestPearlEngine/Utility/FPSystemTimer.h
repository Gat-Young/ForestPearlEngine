#pragma once
#include <chrono>
#include <string>
#include <unordered_map>

// Engine과 연결되지 않은 독립적인 SingleTone 프로파일링 타이머
class FPSystemTimer
{
	public :
		static FPSystemTimer& Get()
		{
			static FPSystemTimer Instance;
			return Instance;
		}

		unsigned int StartProfileTimer();
		void EndProfileTimer(std::string LogText, unsigned int ProfileID);

	private :
		FPSystemTimer() = default;

	private :
		std::unordered_map<unsigned int, std::chrono::steady_clock::time_point> ProfileTimerMap;
		unsigned int NextProfileID = 0;
};
