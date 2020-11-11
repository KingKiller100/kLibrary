#pragma once

#include "../Stopwatch/kStopwatch.hpp"

#include <functional>
#include <string>
#include <thread>

namespace klib::kProfiler
{
	struct ProfilerResult
	{
		const std::string name;
		int64_t start, end;
		uint32_t threadID;
	};

	template<typename TimeUnits = kStopwatch::units::Millis, typename ProfilerFunc = std::function<void(const ProfilerResult&)>>
	class Profiler
	{
	private:
		using TimeUnitsT = TimeUnits;
		using FuncT = ProfilerFunc;

	public:
		Profiler(const std::string_view& name, FuncT&& cb)
			: result({ name.data(), 0, 0, 0 }), isRunning(true),
			callback(std::forward<FuncT>(cb)),
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
			result.end = timer.Now<TimeUnitsT>();
			result.start = timer.GetStartTime<TimeUnitsT>();
			result.threadID = static_cast<uint32_t>(
				std::hash<std::thread::id>{}
			( std::this_thread::get_id() ));

			callback(result);

			isRunning = false;
		}

	private:
		ProfilerResult result;
		bool isRunning;
		FuncT callback;

		kStopwatch::Stopwatch<int64_t> timer;
	};
}
