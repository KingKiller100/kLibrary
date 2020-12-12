#include "pch.hpp"
#include "DebugHelp_Test.hpp"

#include "../../Source Files/Utility/Debug/kDebugger.hpp"
#include "../../Source Files/Utility/Debug/kAssert.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{	
	DebugTester::DebugTester()
		: TesterBase("Debug Help Test")
	{}

	DebugTester::~DebugTester()
		= default;

	using namespace klib::kDebug;
	
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
		IsDebuggerAttached("DebugTest");
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
		FailedConditionException("Assert", "Working Great!", __FILE__, __LINE__,
			[&](std::string&, const SourceInfo&)
			{
				VERIFY(true);
			});
		return success;
	}
}
#endif
