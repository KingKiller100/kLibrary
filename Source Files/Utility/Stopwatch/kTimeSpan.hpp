#pragma once

#include <chrono>

namespace klib
{
	namespace kStopwatch
	{
		struct TimeSpan
		{
		public:
			explicit TimeSpan(const std::chrono::hours h, const std::chrono::minutes m
				, const std::chrono::seconds s
				, const std::chrono::milliseconds ms);

			std::chrono::hours hours;
			std::chrono::minutes minutes;
			std::chrono::seconds seconds;
			std::chrono::milliseconds milliseconds;
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kStopwatch;
#endif
}
