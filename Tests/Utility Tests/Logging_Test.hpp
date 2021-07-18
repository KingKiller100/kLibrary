#pragma once

#include "../../Source/Testing/TesterBase.hpp"
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
		void CleanUp() override;
	
	private:
		void LogTest();
		void DummyLoggerTest();

	private:
		std::filesystem::path fullFilePathToDelete;
	};
}
#endif

