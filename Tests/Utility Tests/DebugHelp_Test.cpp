#include "pch.hpp"
#include "DebugHelp_Test.hpp"

#include "../../Source Files/Utility/Debug Helper/kAssert.hpp"
#include "../../Source Files/Utility/Debug Helper/kDebugger.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{	
	DebugHelpTester::DebugHelpTester()
		: Tester("Debug Help Test")
	{
	}

	DebugHelpTester::~DebugHelpTester()
		= default;

	void DebugHelpTester::Test()
{
		klib::kDebug::CheckRemoteDebuggerAttached("DebugTest");
		//klib::kDebug::BreakPointHere(); // Works Great!
		//klib::kAssert(false, "Working Great!"); 
	}
}
#endif
