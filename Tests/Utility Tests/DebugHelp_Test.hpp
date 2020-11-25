#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class DebugHelpTester : public TesterBase
	{
	public:
		DebugHelpTester();
		~DebugHelpTester() override;
		
	protected:
		void Test() override;
	};
}
#endif


