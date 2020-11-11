#include "pch.hpp"
#include "Logging_Test.hpp"


#include "../../Source Files/Utility/File System/kFileSystem.hpp"
#include "../../Source Files/Utility/Logging/kLogging.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	LoggingTester::LoggingTester()
		: Tester("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Test()
	{
		VERIFY(LogTest() == true);

		std::filesystem::remove_all(fullFilePathToDelete);
	}

	bool LoggingTester::LogTest()
	{
		using namespace klib::kLogs;

		const auto filename = "DiffFileName";
		const auto dir = std::filesystem::current_path().string() + "\\Test Results\\Log Test Dir\\";

		auto testLogger = std::make_unique<Logging>(dir, filename);

		testLogger->ToggleConsoleEnabled();
		testLogger->OutputInitialized("Welcome to logging test");
		testLogger->SuspendFileLogging();

		testLogger->SetCacheMode(true);

		{
			testLogger->AddBanner("TEST", "BANNER!",
			                      "*", "*", 12);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText("BANNER!"));
			VERIFY(last.HasDescription("TEST"));
		}

		testLogger->SetMinimumLoggingLevel(LogLevel::NORM);

		{
			testLogger->AddEntry(LogLevel::DBUG, "DEBUG!");
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(!last.HasText("DEBUG!"));
			VERIFY(!last.HasDescription(LogLevel::DBUG));
		}

		testLogger->SetMinimumLoggingLevel(LogLevel::DBUG);

		{
			constexpr char msg[] = "DEBUG!";
			constexpr auto desc = LogLevel::DBUG;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "NORMAL!";
			constexpr auto desc = LogLevel::NORM;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "INFORMATIVE!";
			constexpr auto desc = LogLevel::INFO;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "WARNING!";
			constexpr auto desc = LogLevel::WARN;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "ERROR!";
			constexpr auto desc = LogLevel::ERRR;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		testLogger->ErasePrevious(1);

		{
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(!last.HasText("ERROR!"));
		}

		{
			constexpr char msg[] = "ERROR AGAIN!";
			constexpr auto desc = LogLevel::ERRR;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		testLogger->ResumeFileLogging();

		testLogger->OutputToFatalFile(LogMessage("FATAL!", __FILE__, __LINE__));

		testLogger->FinalOutput();

		fullFilePathToDelete = dir + filename + ".log";
		VERIFY(std::filesystem::exists(fullFilePathToDelete.c_str()) == true);

		{
			constexpr char msg[] = "END!";
			constexpr auto desc = LogLevel::NORM;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(!last.HasText(msg));
			VERIFY(!last.HasDescription(desc));
		}

		return success;
	}
}
#endif
