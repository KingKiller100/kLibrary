#include "pch.hpp"
#include "kTimeSpan.hpp"


namespace klib::kStopwatch
{
	TimeSpan::TimeSpan(const std::chrono::hours h, const std::chrono::minutes m, const std::chrono::seconds s,
		const std::chrono::milliseconds ms)
		: hours(h)
		, minutes(m)
		, seconds(s)
		, milliseconds(ms)
	{}
}
