﻿#include "pch.hpp"
#include "Profiler_Test.hpp"

#include "../../Source Files/Utility/Profiler/kProfiler.hpp"
#include "../../Source Files/Maths/kMathsApproximately.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	ProfilerTester::ProfilerTester()
		: TesterBase("Profiler Test")
	{}

	ProfilerTester::~ProfilerTester()
	{}

	using namespace klib::kProfiler;
	using namespace klib::kStopwatch;
	using namespace std::chrono_literals;
	
	void ProfilerTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(MillisecondsTest());
		VERIFY_MULTI(SecondsTest());
		VERIFY_MULTI_END();
	}

	bool ProfilerTester::MillisecondsTest()
	{
		{
			using ScopeProfiler = Profiler<long long, units::Millis>;
			const auto name = "Milliseconds Test";
			ScopeProfiler profiler(name, [this, &name](const ScopeProfiler::Result_t& results)
				{
					VERIFY(results.name == name);
					const auto duration = static_cast<double>(results.end - results.start);
					VERIFY(kmaths::Approximately(duration, 250, 50));
					const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
					VERIFY(hashThrID == results.threadID);
				});
			std::this_thread::sleep_for(250ms);
		}

		{
			using ScopeProfiler = Profiler<long long, units::Millis>;
			const auto name = "Milliseconds Test";
			ScopeProfiler profiler(name, [this, &name](const ScopeProfiler::Result_t& results)
				{
					VERIFY(results.name == name);
					const auto duration = static_cast<double>(results.end - results.start);
					VERIFY(kmaths::Approximately(duration, 750, 50));
					const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
					VERIFY(hashThrID == results.threadID);
				});
			std::this_thread::sleep_for(750ms);
		}

		return success;
	}

	bool ProfilerTester::SecondsTest()
	{
		{
			using ScopeProfiler = Profiler<long long, units::Secs>;
			const auto name = "Seconds Test";
			ScopeProfiler profiler(name, [this, &name](const ScopeProfiler::Result_t& results)
				{
					VERIFY(results.name == name);
					const auto duration = static_cast<double>(results.end - results.start);
					VERIFY(kmaths::Approximately(duration, 2, 0));
					const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
					VERIFY(hashThrID == results.threadID);
				});
			std::this_thread::sleep_for(2s);
		}

		{
			using ScopeProfiler = Profiler<long long, units::Millis>;
			const auto name = "Seconds Test";
			ScopeProfiler profiler(name, [this, &name](const ScopeProfiler::Result_t& results)
				{
					VERIFY(results.name == name);
					const auto duration = static_cast<double>(results.end - results.start);
					VERIFY(kmaths::Approximately(duration, 2500, 50));
					const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
					VERIFY(hashThrID == results.threadID);
				});
			std::this_thread::sleep_for(2.5s);
		}

		return success;
	}
}
#endif

