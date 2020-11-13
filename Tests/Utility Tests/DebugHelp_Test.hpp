#pragma once
#include "../../Source Files/Testing/Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class DebugHelpTester : public Tester
	{
	public:
		DebugHelpTester();
		~DebugHelpTester() override;
		
	protected:
		void Test() override;
	};
}
#endif


