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

	template<typename TimeUnits = kStopwatch::units::Millis, class Representation = std::uint64_t, typename ProfilerFunc = std::function<void(const ProfilerResult<Representation>&)>>
	class Profiler
	{
	private:
		using TimeUnitsT = TimeUnits;
		using FuncT = ProfilerFunc;
		using RepT = Representation;
		
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
			result.end = timer.template Now<TimeUnitsT>();
			result.start = timer.template GetStartTime<TimeUnitsT>();
			result.threadID = static_cast<uint32_t>(
				std::hash<std::thread::id>{}
			( std::this_thread::get_id() ));

			callback(result);

			isRunning = false;
		}

	private:
		ProfilerResult<RepT> result;
		bool isRunning;
		FuncT callback;

		kStopwatch::Stopwatch<RepT> timer;
	};
}
