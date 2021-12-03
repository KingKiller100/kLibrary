#include "pch.hpp"
#include "Logging_Test.hpp"

#include "../../Source/Utility/Calendar/kCalendarToString.hpp"
#include "../../Source/Utility/Logging/kLogging.hpp"
#include "../../Source/Utility/String/kToString.hpp"
#include "../../Source/Template/kToImpl.hpp"

#include <sstream>

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	LoggingTester::LoggingTester()
		: TesterBase("Logging Test")
	{	}

	using namespace klib::kLogs;

	LoggingTester::~LoggingTester()
		= default;

	void LoggingTester::Prepare() noexcept
	{
		ADD_TEST(LogTest());
		ADD_TEST(DummyLoggerTest());
	}

	void LoggingTester::CleanUp()
	{
		std::filesystem::remove_all(fullFilePathToDelete);
		std::filesystem::remove(fullFilePathToDelete.parent_path());
	}

	class DummyLogger : public LogDestWithFormatSpecifier
	{
	public:
		DummyLogger()
			: active(false)
			, name(nullptr)
		{
			LogDestWithFormatSpecifier::SetFormat("&t", LogLevel::RAW);
		}
		
		~DummyLogger() override {}
		
		void SetName(std::string* newName) override
		{
			name = newName;
		}
		
		void AddEntry(const LogEntry& entry) override
		{
			if (!active)
				return;
			
			const auto& msg = entry.GetMsg();
			const auto& desc = entry.GetDescriptor();
			
			// Message details
			const auto& t = msg.time;
			const auto& hour = t.GetHour();
			const auto& minute = t.GetMinute();
			const auto& second = t.GetSecond();
			const auto& milli = t.GetMillisecond();

			const auto& d = msg.date;
			const auto& day = d.GetDay();
			const auto& month = d.GetMonth();
			const auto& year = d.GetYear();

			const auto& text = msg.text;

			const auto& sourceInfo = msg.sourceInfo;

			// Description details
			const auto lvl = desc.lvl;
			const auto info = desc.info;

			const auto format = formatMap.at(lvl);

			std::string logLine = klib::kString::ToString(format,
			                                              day,
			                                              month,
			                                              year,
			                                              hour,
			                                              minute,
			                                              second,
			                                              milli,
			                                              *name,
			                                              desc.info,
			                                              desc.lvl.ToUnderlying(),
			                                              text,
			                                              sourceInfo.file,
			                                              sourceInfo.line,
			                                              sourceInfo.func
			);

			logLine.push_back('\n');

			Flush(logLine);
		}
		
		bool IsOpen() const override
		{
			return active;
		}
		
		bool Open() override
	{
			active = true;
			return active;
	}
		void Close(const bool outputClosingMsg) override
		{
			active = false;
		}

		std::string GetLogs() const
		{
			return buffer.str();
		}
		
	private:
		void Flush(const std::string& logLine)
		{
			buffer << logLine;
		}
		
	private:
		bool active;
		std::string* name;
		std::stringstream buffer;
	};

	void LoggingTester::LogTest()
	{
		const std::string filename = "DiffFileName";
		auto dir = std::filesystem::current_path();
		dir /= "Test Results";
		dir /= "Log Test Dir";
		const auto* const extension = ".log";
		const std::filesystem::path path = dir / (filename + extension);

		auto testLogger = std::make_unique<LogDispatcher>(path);

		testLogger->GetFile().SetFormat("[&N] [&p]: &t", LogLevel::BNR);
		testLogger->AddBanner( , , "Welcome to logging test", "*", "*", 20 );
		testLogger->GetFile().Close(false);

		{
			testLogger->AddBanner( , ,
				"BANNER!", "*", "*", 12 );
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText("BANNER!"));
			VERIFY(last.HasDescription("TEST"));
		}

		testLogger->SetGlobalLevel(LogLevel::NRM);

		{
			testLogger->AddEntry( , "DEBUG!" );
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(!last.HasText("DEBUG!"));
			VERIFY(!last.HasDescription(LogLevel::DBG));
		}

		testLogger->SetGlobalLevel(LogLevel::DBG);

		{
			constexpr char msg[] = "DEBUG!";
			constexpr auto desc = LogLevel::DBG;
			testLogger->AddEntry( , msg );
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "NORMAL!";
			constexpr auto desc = LogLevel::NRM;
			testLogger->AddEntry( , msg );
			const auto hasCache = testLogger->HasCache();
			VERIFY(hasCache);
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "INFORMATIVE!";
			constexpr auto desc = LogLevel::INF;
			testLogger->AddEntry( , msg );
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "WARNING!";
			constexpr auto desc = LogLevel::WRN;
			testLogger->AddEntry( , msg );
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		{
			constexpr char msg[] = "ERROR!";
			constexpr auto desc = LogLevel::ERR;
			testLogger->AddEntry( , msg );
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
			testLogger->AddEntry( , msg );
			const auto& last = testLogger->GetLastCachedEntry();
			VERIFY(last.HasText(msg));
			VERIFY(last.HasDescription(desc));
		}

		testLogger->GetFile().Open();
		testLogger->AddFatal(LogMessage("FATAL!", __FILE__, __LINE__));
		testLogger->FinalOutput(false);

		fullFilePathToDelete = dir / (filename + extension);
		VERIFY(std::filesystem::exists(fullFilePathToDelete.c_str()) == true);

		{
			constexpr char msg[] = "END!";
			constexpr auto desc = LogLevel::NRM;
			testLogger->AddEntry( , msg );
			const auto hasCache = testLogger->HasCache();
			VERIFY(!hasCache);

			bool crashWhenTryingToAccessEmptyCache;
			
			try
			{
				const auto& last = testLogger->GetLastCachedEntry();
				VERIFY(!last.HasText(msg));
				VERIFY(!last.HasDescription(desc));
				crashWhenTryingToAccessEmptyCache = false;
			}
			catch (...)
			{
				crashWhenTryingToAccessEmptyCache = true;
			}

			VERIFY(crashWhenTryingToAccessEmptyCache);
		}

		
	}

	void LoggingTester::DummyLoggerTest()
	{
		LogDispatcher dummy(std::filesystem::current_path() / "Dummy", "Dummy");
		dummy.AddDestination<DummyLogger>();
		
		auto& dest = dummy.GetExtraDestination<DummyLogger>(0);
		dest.Open();
		dest.SetFormat("&n: [&p]: &t", LogLevel::NRM);
		dummy.AddEntry( , "Normal test" );
		dummy.Flush();

		VERIFY(dest.IsOpen());
		const auto logs = dest.GetLogs();
		VERIFY(!logs.empty());
		VERIFY(logs == "Dummy: [NRM]: Normal test\n")
		dest.Close(false);
		VERIFY(!dest.IsOpen());

		
		
	}
}
#endif
