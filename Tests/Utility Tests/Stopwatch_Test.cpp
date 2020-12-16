#include "pch.hpp"
#include "../Utility Tests/Stopwatch_Test.hpp"

#include "../../Source Files/Maths/kMathsApproximately.hpp"
#include "../../Source Files/Utility/Stopwatch/kStopwatch.hpp"
#include "../../Source Files/Utility/String/kToString.hpp"

#include <thread>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StopWatchTester::StopWatchTester()
		: TesterBase("Stopwatch Test")
	{	}

	StopWatchTester::~StopWatchTester()
		= default;

	using namespace klib::kStopwatch;
	using namespace std::chrono_literals;
	
	void StopWatchTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(OneSecondTest());
		VERIFY_MULTI_END();
	}

	bool StopWatchTester::OneSecondTest()
	{
		Stopwatch<std::time_t, HighAccuracyClock<units::Millis>> sw("500ms Timer");
		std::this_thread::sleep_for(500ms);
		const auto now = sw.GetDeltaTime();
		VERIFY(kmaths::Approximately(now, 500, 1));

		return success;
	}
}
#endif
