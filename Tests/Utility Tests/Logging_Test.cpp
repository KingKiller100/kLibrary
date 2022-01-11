#include "Logging_Test.hpp"

#include "../../Source/Utility/Logging/kLogging.hpp"
#include "../../Source/Utility/Logging/Destinations/kiLoggerDestination.hpp"
#include "../../Source/Utility/Logging/Destinations/kFileLogger.hpp"

#include "../../Source/Utility/Logging/kLogEntry.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	LoggingTester::LoggingTester()
		: TesterBase( "Logging Test" )
	{ }

	using namespace klib::kLogs;

	LoggingTester::~LoggingTester()
	= default;

	void LoggingTester::Prepare() noexcept
	{
		ADD_TEST( LogTest() );
	}

	class CacheLogger : public FormattedLogDestinationBase
	{
	public:
		CacheLogger()
			: active( false )
		{
			FormattedLogDestinationBase::SetRawFormat( "&t" );
		}

		~CacheLogger() override
		= default;

		void AddEntry( const LogEntry& entry ) override
		{
			if ( !active )
				return;
			Flush( entry );
		}

		bool IsOpen() const override
		{
			return active;
		}

		void Open() override
		{
			active = true;
		}

		void Close() override
		{
			active = false;
			Clear();
		}

		[[nodiscard]] const LogEntry& GetLastEntry() const
		{
			if ( entries.empty() )
				throw std::out_of_range( "No log entries left" );
			return entries.back();
		}

		void Pop()
		{
			entries.pop_back();
		}

		void Clear()
		{
			entries.clear();
		}

		std::string_view GetName() const override
		{
			return "Cache Logger";
		}

		void AddRaw( const LogMessage& message ) override
		{ }

	private:
		void Flush( const LogEntry& message )
		{
			entries.emplace_back( message );
		}

		bool active;
		std::vector<LogEntry> entries;
	};

	void LoggingTester::LogTest()
	{
		LogDispatcher dispatcher;
		auto& destination = dispatcher.AddDestination<CacheLogger>().Ref<CacheLogger>();
		const auto profile = dispatcher.RegisterProfile( "Test", LogLevel::DBG );

		VERIFY( profile.GetLevel() == LogLevel::DBG );

		destination.SetFormat( "[&N] [&p]: &t" );
		profile.AddBanner( "Welcome to logging test", "*", "*", 20 );

		VERIFY_THROWS( destination.GetLastEntry() );

		dispatcher.Open();

		{
			profile.AddBanner( "BANNER!", "*", "*", 12 );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText("BANNER!") );
			VERIFY( last.GetMsg().text == "************BANNER!************" );
		}

		dispatcher.SetGlobalLevel( LogLevel::ERR );

		{
			VERIFY( profile.GetLevel() == LogLevel::ERR );
		}

		dispatcher.SetGlobalLevel( LogLevel::TRC );

		{
			constexpr char msg[] = "TRACE!";
			constexpr auto desc = LogLevel::TRC;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() == desc );
		}

		{
			constexpr char msg[] = "DEBUG!";
			constexpr auto desc = LogLevel::DBG;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() != desc );
		}

		{
			constexpr char msg[] = "INFORMATIVE!";
			constexpr auto desc = LogLevel::INF;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() != desc );
		}

		{
			constexpr char msg[] = "WARNING!";
			constexpr auto desc = LogLevel::WRN;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() != desc );
		}

		{
			constexpr char msg[] = "ERROR!";
			constexpr auto desc = LogLevel::ERR;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() != desc );
		}

		destination.Pop();

		{
			const auto& last = destination.GetLastEntry();
			VERIFY( !last.HasText("ERROR!") );
			VERIFY( last.GetProfile().GetLevel() != LogLevel::ERR );
		}

		{
			constexpr char msg[] = "ERROR AGAIN!";
			constexpr auto desc = LogLevel::ERR;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() != desc );
		}

		{
			constexpr char msg[] = "FATAL!";
			constexpr auto desc = LogLevel::FTL;
			profile.AddEntry( desc, msg );
			const auto& last = destination.GetLastEntry();
			VERIFY( last.HasText(msg) );
			VERIFY( last.GetProfile().GetLevel() != desc );
		}

		dispatcher.Close();

		{
			constexpr char msg[] = "END!";
			constexpr auto desc = LogLevel::INF;
			profile.AddEntry( desc, msg );
			VERIFY_THROWS( destination.GetLastEntry() );
		}
	}
}
#endif
