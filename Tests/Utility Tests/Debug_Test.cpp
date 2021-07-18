#include "pch.hpp"
#include "Debug_Test.hpp"

#ifdef TESTING_ENABLED

#include "../../Source/Utility/Debug/kDebugger.hpp"
#include "../../Source/Utility/Debug/kAssert.hpp"

namespace kTest::utility
{
	DebugTester::DebugTester()
		: TesterBase("Debug Test")
	{}

	DebugTester::~DebugTester()
		= default;

	using namespace klib::kDebug;
	using namespace std::chrono_literals;

	void DebugTester::Prepare() noexcept
	{
		ADD_TEST(IsDebuggerAttachedTest());
		ADD_TEST(BreakPointTest());
		ADD_TEST(FailedConditionExceptionTest());
	}

	void DebugTester::IsDebuggerAttachedTest()
	{
		constexpr auto waitTime = 1000ms;
		using Duration = std::remove_const_t<decltype(waitTime)>;

		const auto startTimePoint = std::chrono::high_resolution_clock::now();
		Duration duration(0);

		ScanForDebugger(waitTime);

		const auto endTimePoint = std::chrono::high_resolution_clock::now();

		duration = std::chrono::duration_cast<Duration>(endTimePoint - startTimePoint);

		if (IsDebuggerAttached())
		{
			VERIFY(waitTime > duration); // Should be a quick check since debugger is attached
		}
		else
		{
			VERIFY(waitTime <= duration); // Should timeout due to no debugger being attached
		}
	}

	void DebugTester::BreakPointTest()
	{
		//klib::kDebug::BreakPoint(); // Works Great!
	}

	void DebugTester::FailedConditionExceptionTest()
	{
		bool failedConditionExceptionTest = false;
		FailedConditionException("Assert", "Working Great!", SOURCE_INFO(),
			[&](std::string&, const SourceInfo&)
		{
			failedConditionExceptionTest = true;
		});

		VERIFY(failedConditionExceptionTest);
	}
}
#endif
