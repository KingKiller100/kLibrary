#include "pch.hpp"
#include "DebugHelp_Test.hpp"

#include "../../Source Files/Utility/Debug/kDebugger.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{	
	DebugHelpTester::DebugHelpTester()
		: TesterBase("Debug Help Test")
	{
	}

	DebugHelpTester::~DebugHelpTester()
		= default;

	void DebugHelpTester::Test()
	{
		klib::kDebug::CheckRemoteDebuggerAttached("DebugTest");
		//klib::kDebug::Break(); // Works Great!
		//klib::kAssert(false, "Working Great!"); 
	}
}
#endif
