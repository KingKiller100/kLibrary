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
	
	void DebugTester::Prepare() noexcept
	{
		VERIFY_MULTI_INIT()
		VERIFY_MULTI(IsDebuggerAttachedTest())
		VERIFY_MULTI(BreakPointTest())
		VERIFY_MULTI(FailedConditionExceptionTest())
		VERIFY_MULTI_END()
	}

	bool DebugTester::IsDebuggerAttachedTest()
	{
		// WaitForDebugger("DebugTest", 1000ms);
		ScanForDebugger(1000ms);
		
	}

	bool DebugTester::BreakPointTest()
	{
		//klib::kDebug::BreakPoint(); // Works Great!
		
	}

	bool DebugTester::FailedConditionExceptionTest()
	{
		success = false;
		FailedConditionException("Assert", "Working Great!", SOURCE_INFO(),
			[&](std::string&, const SourceInfo&)
			{
				success = true;
			});
		
	}
}
#endif
