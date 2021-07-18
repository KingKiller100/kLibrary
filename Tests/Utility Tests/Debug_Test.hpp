#pragma once
#include "../../Source/Testing/TesterBase.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class DebugTester : public TesterBase
	{
	public:
		DebugTester();
		~DebugTester() override;
		
	protected:
		void Prepare() noexcept override;

	private:
		void IsDebuggerAttachedTest();
		void BreakPointTest();
		void FailedConditionExceptionTest();
	};
}
#endif


