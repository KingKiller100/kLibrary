#include "pch.hpp"
#include "Debug_Test.hpp"

#include "../../Source/Utility/Debug/kDebugger.hpp"
#include "../../Source/Utility/Debug/kAssert.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{	
	DebugTester::DebugTester()
		: TesterBase("Debug Test")
	{}

	DebugTester::~DebugTester()
		= default;

	using namespace klib::kDebug;
	using namespace std::chrono_literals;
	
	void DebugTester::Test()
	{
		VERIFY_MULTI_INIT()
		VERIFY_MULTI(IsDebuggerAttachedTest())
		VERIFY_MULTI(BreakPointTest())
		VERIFY_MULTI(FailedConditionExceptionTest())
		VERIFY_MULTI_END()
	}

	bool DebugTester::IsDebuggerAttachedTest()
	{
		IsDebuggerAttached("DebugTest", 1000ms);
		return success;
	}

	bool DebugTester::BreakPointTest()
	{
		//klib::kDebug::BreakPoint(); // Works Great!
		return success;
	}

	bool DebugTester::FailedConditionExceptionTest()
	{
		success = false;
		FailedConditionException("Assert", "Working Great!", SOURCE_INFO(),
			[&](std::string&, const SourceInfo&)
			{
				success = true;
			});
		return success;
	}
}
#endif
