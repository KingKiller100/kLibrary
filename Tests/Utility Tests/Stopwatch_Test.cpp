#include "pch.hpp"
#include "../Utility Tests/Stopwatch_Test.hpp"

#include "../../Source Files/Maths/kAlgorithms.hpp"
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
		Stopwatch<std::time_t, HighAccuracyClock<units::Secs>> oneSec("Timer");
		std::this_thread::sleep_for(1s);
		const auto now = oneSec.GetDeltaTime();
		VERIFY(kmaths::constants::ApproximatelyOne(now));

		return success;
	}
}
#endif
