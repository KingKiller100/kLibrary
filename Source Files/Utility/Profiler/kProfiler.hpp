#pragma once

#include "../Stopwatch/kStopwatch.hpp"

#include <functional>
#include <string>
#include <cstdint>
#include <thread>

namespace klib::kProfiler
{
	template<class Representation>
	struct ProfilerResult
	{
		const std::string name;
		Representation start, end;
		uint32_t threadID{};
	};

	template<typename TimeUnits = kStopwatch::units::Millis, class Representation = std::int64_t, typename ProfilerFunc = std::function<void(const ProfilerResult<Representation>&)>>
	class Profiler
	{
	private:
		using TimeUnits_t = TimeUnits;
		using Func_t = ProfilerFunc;
		using Rep_t = Representation;
		
	public:
		Profiler(const std::string_view& name, Func_t&& cb)
			: result({ name.data(), 0, 0, 0 }), isRunning(true),
			callback(std::forward<Func_t>(cb)),
			timer("Profiler")
		{}

		~Profiler()
		{
			if (isRunning)
				Stop();
		}

	private:
		void Stop()
		{
			result.end = timer.template Now<TimeUnits_t>();
			result.start = timer.template GetStartTime<TimeUnits_t>();
			result.threadID = static_cast<uint32_t>(
				std::hash<std::thread::id>{}
			( std::this_thread::get_id() ));

			callback(result);

			isRunning = false;
		}

	private:
		ProfilerResult<Rep_t> result;
		bool isRunning;
		Func_t callback;

		kStopwatch::Stopwatch<Rep_t> timer;
	};
}
