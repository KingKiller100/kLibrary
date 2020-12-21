#include "pch.hpp"
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
		VERIFY_MULTI(MicrosecondsTest());
		VERIFY_MULTI(MillisecondsTest());
		VERIFY_MULTI(SecondsTest());
		VERIFY_MULTI_END();
	}

	bool ProfilerTester::MicrosecondsTest()
	{
		{
			using ScopeProfiler = Profiler<long long>;
			const auto name = "Micros Test";
			ScopeProfiler profiler(name, [this, name](const ScopeProfiler::Result_t& results)
				{
					VERIFY(results.name == name);
					const auto duration = static_cast<double>(results.end - results.start);
					VERIFY(kmaths::Approximately(duration, 2000, 10));
					const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
					VERIFY(hashThrID == results.threadID);
				});
			std::this_thread::sleep_for(2ms);
		}
		
		return success;
	}

	bool ProfilerTester::MillisecondsTest()
	{
		{
			using ScopeProfiler = Profiler<long long, units::Micros>;
			const auto name = "Millis Test";
			ScopeProfiler profiler(name, [this, name](const ScopeProfiler::Result_t& results)
				{
					VERIFY(results.name == name);
					const auto duration = results.end - results.start;
					VERIFY(kmaths::Approximately(duration, 2000, 10));
					const auto hashThrID = std::hash<std::thread::id>{}(std::this_thread::get_id());
					VERIFY(hashThrID == results.threadID);
				});
			std::this_thread::sleep_for(2ms);
		}
		
		return success;
	}

	bool ProfilerTester::SecondsTest()
	{
		return success;
	}
}
#endif

