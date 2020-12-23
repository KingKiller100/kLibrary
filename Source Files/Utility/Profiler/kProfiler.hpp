#pragma once

#include "../Stopwatch/Clock/kClock.hpp"
#include "../Thread/kThreadID.hpp"

#include <functional>
#include <string>
#include <cstdint>

namespace klib {
	namespace kProfiler
	{
		template<class CharType, class Representation>
		struct BasicProfilerResult
		{
			using Rep_t = Representation;

			const std::basic_string<CharType> name;
			std::uint32_t threadID;
			Rep_t end, start;

			BasicProfilerResult(const std::basic_string<CharType>& name, Rep_t startTime, std::uint32_t thrId = 0)
				: name(name)
				, threadID(thrId)
				, start(startTime)
			{}
		};

		template<class CharType,
			class Representation = size_t,
			typename TimeUnits = kStopwatch::units::Millis,
			class Clock = kStopwatch::HighAccuracyClock<TimeUnits>,
			typename ProfilerFunc = std::function<void(const BasicProfilerResult<CharType, Representation>&)>>
			class BasicScopeProfiler
		{
		public:
			using Clock_t = Clock;
			using Func_t = ProfilerFunc;
			using Rep_t = Representation;
			using TimeUnits_t = TimeUnits;
			using Result_t = BasicProfilerResult<CharType, Rep_t>;

		public:
			BasicScopeProfiler(const std::basic_string<CharType>& name, Func_t&& cb)
				: isRunning(true)
				, callback(std::forward<Func_t>(cb))
				, result(name, Now(), kThread::GetThreadID<std::uint32_t>())
			{}

			BasicScopeProfiler(const std::basic_string_view<CharType>& name, Func_t&& cb)
				: isRunning(true)
				, callback(std::forward<Func_t>(cb))
				, result(name.data(), Now(), kThread::GetThreadID<std::uint32_t>())
			{}

			BasicScopeProfiler(const CharType* name, Func_t&& cb)
				: isRunning(true)
				, callback(std::forward<Func_t>(cb))
				, result(name, Now(), kThread::GetThreadID<std::uint32_t>())
			{}

			~BasicScopeProfiler()
			{
				result.end = Now();

				callback(result);

				isRunning = false;
			}

		private:
			Rep_t Now() const
			{
				return kStopwatch::DurationTo<Rep_t, Clock_t::Units_t>(Clock_t::Now().time_since_epoch());
			}

		private:
			bool isRunning;
			Func_t callback;
			Result_t result;
		};

		template<class Representation = size_t,
			typename TimeUnits = kStopwatch::units::Millis,
			class Clock = kStopwatch::HighAccuracyClock<TimeUnits>,
			typename ProfilerFunc = std::function<void(const BasicProfilerResult<char, Representation>&)>>
			using Profiler = BasicScopeProfiler<char, Representation, TimeUnits, Clock, ProfilerFunc>;

		template<class Representation>
		using ProfilerResult = BasicProfilerResult<char, Representation>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kProfiler;
#endif
}

