﻿
#include "Profiler_Test.hpp"

#ifdef TESTING_ENABLED
#include "../../Source/Utility/Profiler/kProfiler.hpp"
#include "../../Source/Maths/kMathsApproximately.hpp"

namespace kTest::utility
{
	ProfilerTester::ProfilerTester()
		: TesterBase("Profiler Test")
	{}

	ProfilerTester::~ProfilerTester()
		= default;

	using namespace klib::kProfiler;
	using namespace klib::kStopwatch;
	using namespace std::chrono_literals;

	void ProfilerTester::Prepare() noexcept
	{
		// ADD_TEST(MillisecondsTest());
		ADD_TEST(SecondsTest());
	}

	void ProfilerTester::MillisecondsTest()
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
	}

	void ProfilerTester::SecondsTest()
	{
		{
			constexpr std::chrono::seconds waitTime(5);

			using ScopeProfiler = Profiler<long long, units::Secs>;
			const auto name = "Seconds Test";
			ScopeProfiler profiler(name, [this, &name, waitTime](const ScopeProfiler::Result_t& results)
			{
				constexpr auto allowance = 0;
				VERIFY(results.name == name);
				const auto duration = static_cast<double>(results.end - results.start);
				const auto expectedMidPt = waitTime.count() + allowance;
				VERIFY(kmaths::Approximately(duration, expectedMidPt, allowance));
				const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
				VERIFY(hashThrID == results.threadID);
			});
			std::this_thread::sleep_for(waitTime);
		}

		{
			constexpr std::chrono::milliseconds waitTime(2500);
			constexpr auto allowance = 50;

			using ScopeProfiler = Profiler<long long, units::Millis>;
			const auto name = "Seconds Test";
			ScopeProfiler profiler(name, [this, &name, waitTime, allowance](const ScopeProfiler::Result_t& results)
			{
				VERIFY(results.name == name);
				const auto duration = static_cast<double>(results.end - results.start);
				const auto expectedMidPt = waitTime.count() + allowance;
				VERIFY(kmaths::Approximately(duration, expectedMidPt, allowance));
				const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
				VERIFY(hashThrID == results.threadID);
			});
			std::this_thread::sleep_for(waitTime);
		}
	}
}
#endif
