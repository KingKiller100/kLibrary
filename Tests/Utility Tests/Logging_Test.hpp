#pragma once

#include "../../Source Files/Testing/Tester.hpp"
#include <filesystem>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class LoggingTester : public Tester
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

