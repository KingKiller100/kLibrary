#include "pch.hpp"
#include "Logging_Test.hpp"

#include "../../Source Files/Utility/Logging/kLogging.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	LoggingTester::LoggingTester()
		: TesterBase("Logging Test")
	{	}

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Test()
	{
		VERIFY(LogTest() == true);

		std::filesystem::remove_all(fullFilePathToDelete);
		const auto stem = fullFilePathToDelete.stem();
		const auto ext = fullFilePathToDelete.extension();
		std::filesystem::remove(fullFilePathToDelete.parent_path());

	}

	bool LoggingTester::LogTest()
	{
		using namespace klib::kLogs;

		const char* filename = "DiffFileName";
		const auto dir = std::filesystem::current_path().string() + "\\Test Results\\Log Test Dir\\";
		const auto* const extension = ".log";
		const std::filesystem::path path = dir + filename + extension;

		auto testLogger = std::make_unique<Logging>(path);

		testLogger->ToggleConsoleEnabled();
		testLogger->SetFileFormat("[&N] [&l]: &t");
		testLogger->AddBanner("Intro", "Welcome to logging test", "*", "*", 20);
		testLogger->SuspendFileLogging();

		testLogger->SetCacheMode(true);

		{
			testLogger->AddBanner("TEST", "BANNER!",
				"*", "*", 12);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText("BANNER!"));
			VERIFY(last.HasDescription("TEST"));
		}

		testLogger->SetMinimumLoggingLevel(LogLevel::NRM);

		{
			testLogger->AddEntry(LogLevel::DBG, "DEBUG!");
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(!last.HasText("DEBUG!"));
			VERIFY(!last.HasDescription(LogLevel::DBG));
		}

		testLogger->SetMinimumLoggingLevel(LogLevel::DBG);

		{
			constexpr char msg[] = "DEBUG!";
			constexpr auto desc = LogLevel::DBG;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "NORMAL!";
			constexpr auto desc = LogLevel::NRM;
			testLogger->AddEntry(desc, msg);
			const auto hasCache = testLogger->HasCache();
			VERIFY(hasCache);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "INFORMATIVE!";
			constexpr auto desc = LogLevel::INF;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "WARNING!";
			constexpr auto desc = LogLevel::WRN;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "ERROR!";
			constexpr auto desc = LogLevel::ERR;
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
			constexpr auto desc = LogLevel::ERR;
			testLogger->AddEntry(desc, msg);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		testLogger->ResumeFileLogging();

		testLogger->AddFatal(LogMessage("FATAL!", __FILE__, __LINE__));

		testLogger->FinalOutput();

		fullFilePathToDelete = dir + filename + extension;
		VERIFY(std::filesystem::exists(fullFilePathToDelete.c_str()) == true);

		{
			constexpr char msg[] = "END!";
			constexpr auto desc = LogLevel::NRM;
			testLogger->AddEntry(desc, msg);
			const auto hasCache = testLogger->HasCache();
			VERIFY(!hasCache);
			bool prevSuccess = success;
			try
			{
				success = false;
				const auto& last = testLogger->GetLastCachedEntry();
				VERIFY(!last.HasText(msg));
				VERIFY(!last.HasDescription(desc));
			}
			catch (...)
			{
				success = prevSuccess;
			}
		}

		return success;
	}
}
#endif
