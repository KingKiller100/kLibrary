#include "TesterManager.hpp"

#ifdef TESTING_ENABLED

#include "TesterBase.hpp"

// Speed Testing
#include "Performance/PerformanceTestManager.hpp"

#include "../kLib.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>
#include <thread>

namespace kTest
{
	using namespace klib;
	using namespace kString;

	TesterManager::TesterManager()
		: threadPool_()
		, success_( true )
	{
		std::cout.precision( 3 );
	}

	TesterManager::~TesterManager()
	{ }

	void TesterManager::Initialize( InitializationRequest initRequest )
	{
		using namespace klib;


		const auto cwd = std::filesystem::path( __argv[0] ).parent_path();
		current_path( cwd );

		path_ = ( cwd / "Test Results" ).string();
		path_ += "/";

		if ( !std::filesystem::exists( path_ ) )
		{
			if ( !std::filesystem::create_directory( path_.c_str() ) )
			{
				throw std::runtime_error( "Test results directory could not be created/found. Please check why!" );
			}
		}

		path_ += "Results.txt";
		std::filesystem::remove( path_.c_str() );

		file_.open( path_, std::ios::out | std::ios::app );
	}

	void TesterManager::Add( TesterBase* test )
	{
		tests_.push( std::unique_ptr<TesterBase>( test ) );
	}

	void TesterManager::RunAll( ResourceUtilization resourceUtilization )
	{
		using namespace std::chrono;

		const auto testCount = tests_.size();

		if ( testCount == 0 )
		{
			std::cout << "TestManager empty\n";
			return;
		}

		const auto maxResources = std::thread::hardware_concurrency();

		const auto threadAvailable = resourceUtilization == ResourceUtilization::All
			                             ? maxResources
			                             : resourceUtilization == ResourceUtilization::Half
			                             ? maxResources >> 1
			                             : 1;

		const auto threadsCount = ( std::min<size_t> )( threadAvailable, testCount );

		std::cout << "Testing: " <<
			( threadsCount > 1 ? "Multi-Threaded" : "Single Threaded" ) <<
			"[" << threadsCount << "]" << "\n";

		ReportDuration( PerformTests( threadsCount ) );;
		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path_ << std::endl;
	}


	std::vector<std::shared_future<TesterManager::TestResult>> TesterManager::PerformTests( size_t noOfThreads )
	{
		size_t index{ 0 };

		std::vector<std::shared_future<TestResult>> futureResults;
		futureResults.reserve( tests_.size() );

		threadPool_.Launch( noOfThreads );

		while ( !tests_.empty() )
		{
			const auto& currentTest = tests_.top();

			futureResults.emplace_back(
				threadPool_.EnqueueJob( [this]( auto test ) -> TestResult
					{
						return this->Run( test );
					}, currentTest
				)
			);

			tests_.pop();
		}

		startTimePoint_ = std::chrono::high_resolution_clock::now();
		return futureResults;
	}

	TesterManager::TestResult TesterManager::Run( std::shared_ptr<TesterBase> test )
	{
		TestResult result;

		const auto startTime = std::chrono::high_resolution_clock::now();
		const auto passed = test->Run();
		const auto endTime = std::chrono::high_resolution_clock::now();

		const auto duration = endTime - startTime;

		result.testName = test->GetName();
		result.passed = passed;
		result.duration = std::chrono::duration_cast<TargetSubDuration_t>( duration );

		const auto durationStr =
			Sprintf( "| Runtime: %.3fms", static_cast<double>( duration.count() ) / 1'000'000.0 );

		result.report = passed
			                ? Sprintf( "Success: %s %s\n\n", result.testName, durationStr )                          // Success Case
			                : Sprintf( "Failure: %s %s\n%s", result.testName, durationStr, test->GetFailureData() ); // Fail Case
		return result;
	}

	void TesterManager::ReportDuration( std::vector<std::shared_future<TestResult>> futureResults )
	{
		std::vector<TestResult> results;
		results.reserve( futureResults.size() );

		for ( auto& fRes : futureResults )
		{
			try
			{
				const auto& result = fRes.get();
				WriteToFile( result.report );
				results.emplace_back( result );
			}
			catch ( const std::exception& e )
			{
				WriteToFile( kString::Sprintf( "A test threw an exception: %s", e.what() ) );
			}
		}

		const auto endTimePoint = std::chrono::high_resolution_clock::now();

		futureResults.clear();

		// Sort fastest -> slowest
		std::ranges::sort( results, []( const TestResult& lhs, const TestResult& rhs )
		{
			return lhs.duration < rhs.duration;
		} );

		for ( const auto& result : results )
		{
			WriteToConsole( result );
		}

		const auto duration = endTimePoint - startTimePoint_;
		const auto totalMilliseconds = std::chrono::duration_cast<TargetDuration_t>( duration ).count();
		const auto millis = totalMilliseconds % TargetDuration_t::period::den;
		const auto secs = totalMilliseconds - millis;
		const auto totalTestTime = CalculateTotalTestTime( results );
		const auto avgTestTime = static_cast<double>( totalTestTime.count() ) / static_cast<double>( results.size() );

		auto timeStr = Sprintf( "Runtime: %us %ums | ", secs, millis );
		timeStr.append( Sprintf( "Total Test Runtime: %ums | ", totalTestTime.count() ) );
		timeStr.append( Sprintf( "Average Test Runtime: %.3fms", avgTestTime ) );

		WriteToFile( timeStr );

		std::cout << "\n" << timeStr << "\n";
	}

	void TesterManager::WriteToConsole( const TesterManager::TestResult& result )
	{
		const auto& pass = result.passed;
		const auto& name = result.testName;
		const auto millis = static_cast<double>( std::chrono::duration_cast<std::chrono::microseconds>( result.duration ).count() ) / TargetDuration_t::period::den;
		auto* const hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

		std::cout << "Ran: " << name << "| ";

		SetConsoleTextAttribute( hConsole, pass
			                                   ? kMisc::ConsoleColour::LIGHT_GREEN
			                                   : kMisc::ConsoleColour::SCARLET_RED );

		std::cout << ( pass ? "Pass" : "Fail" );

		SetConsoleTextAttribute( hConsole, kMisc::ConsoleColour::LIGHT_GREY );

		std::cout << " | " << millis << "ms\n";
	}

	void TesterManager::WriteToFile( std::string_view results )
	{
		file_ << results;
		file_.flush();
	}

	TesterManager::TargetDuration_t TesterManager::CalculateTotalTestTime( const std::vector<TestResult>& results ) const
	{
		TargetSubDuration_t totalDuration{ 0 };
		for ( const auto& result : results )
		{
			totalDuration += result.duration;
		}

		return std::chrono::duration_cast<TargetDuration_t>( totalDuration );
	}

	void TesterManager::RunPerformanceTests() const
	{
		if ( !success_ )
			return;

		auto& test = performance::PerformanceTestManager::Get();
		std::cout << "Now Testing: " << test.GetName() << " ";
		test.Run();
	}

	void TesterManager::Shutdown()
	{
		if ( file_.is_open() )
			file_.close();

		threadPool_.Shutdown();
		ClearAllTests();
	}

	void TesterManager::ClearAllTests()
	{
		while ( !tests_.empty() )
		{
			tests_.pop();
		}
	}
}
#endif // TESTING_ENABLED
