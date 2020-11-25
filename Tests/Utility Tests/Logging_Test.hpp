#pragma once

#include "../../Source Files/Testing/TesterBase.hpp"
#include <filesystem>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class LoggingTester : public TesterBase
	{
	public:
		LoggingTester();
		~LoggingTester() override;
		
	private:
		void Test() override;
		bool LogTest();

	private:
		std::filesystem::path fullFilePathToDelete;
	};
}
#endif

