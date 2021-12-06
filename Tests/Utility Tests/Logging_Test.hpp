#pragma once

#include "../IncludeTesterBase.hpp"
#include <filesystem>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class LoggingTester : public TesterBase
	{
	public:
		LoggingTester();
		~LoggingTester() override;

	protected:
		void Prepare() noexcept override;
	
	private:
		void LogTest();
	};
}
#endif

