#include "pch.hpp"
#include "kTimeSpan.hpp"


namespace klib::kStopwatch
{
	TimeSpan::TimeSpan(const std::chrono::hours h, const std::chrono::minutes m, const std::chrono::seconds s,
		const std::chrono::milliseconds ms) noexcept
		: hours(h)
		, minutes(m)
		, seconds(s)
		, milliseconds(ms)
	{}

	TimeSpanFull::TimeSpanFull(const std::chrono::hours h, const std::chrono::minutes m, const std::chrono::seconds s,
		const std::chrono::milliseconds ms, const std::chrono::microseconds us, const std::chrono::nanoseconds ns) noexcept
		: hours(h)
		, minutes(m)
		, seconds(s)
		, milliseconds(ms)
		, microseconds(us)
		, nanoseconds(ns)
	{}
}
