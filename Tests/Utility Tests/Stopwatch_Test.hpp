#pragma once
#include "../../Source Files/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StopWatchTester : public TesterBase
	{
	public:
		StopWatchTester();
		~StopWatchTester() override;
		
	protected:
		void Test() override;
	};
}
#endif
