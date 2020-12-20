#pragma once

#include "../Stopwatch/Clock/kClock.hpp"
#include "../Thread/kThreadID.hpp"

#include <functional>
#include <string>
#include <cstdint>

namespace klib {
	namespace kProfiler
	{
		template<class Representation>
		struct ProfilerResult
		{
			using Rep_t = Representation;

			const std::string name;
			Rep_t start, end;
			std::uint32_t threadID;
		};

		template<class CharType,
			typename TimeUnits = kStopwatch::units::Millis,
			class Representation = size_t,
			class Clock = kStopwatch::HighAccuracyClock<TimeUnits>,
			typename ProfilerFunc = std::function<void(const ProfilerResult<Representation>&)>>
			class BasicScopeProfiler
		{
		private:
			using TimeUnits_t = TimeUnits;
			using Func_t = ProfilerFunc;
			using Rep_t = Representation;
			using Clock_t = Clock;

		public:
			BasicScopeProfiler(const std::basic_string_view<CharType>& name, Func_t&& cb)
				: result({ name.data(), Now(), 0, 0 })
				, isRunning(true)
				, callback(std::forward<Func_t>(cb))
			{}

			~BasicScopeProfiler()
			{
				if (isRunning)
					Stop();
			}

		private:
			void Stop()
			{
				result.end = Now();
				result.threadID = kThread::GetThreadID<std::uint32_t>();

				callback(result);

				isRunning = false;
			}

			Rep_t Now() const
			{
				return kStopwatch::TimePointTo<Rep_t, Clock_t>(Clock_t::Now());
			}

		private:
			ProfilerResult<Rep_t> result;
			bool isRunning;
			Func_t callback;
		};

		template<typename TimeUnits = kStopwatch::units::Millis,
			class Representation = size_t,
			class Clock = kStopwatch::HighAccuracyClock<TimeUnits>,
			typename ProfilerFunc = std::function<void(const ProfilerResult<Representation>&)>>
			using Profiler = BasicScopeProfiler<char, TimeUnits, Representation, Clock, ProfilerFunc>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kProfiler;
#endif
}
	
