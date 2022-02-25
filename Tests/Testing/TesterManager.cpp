#include "TesterManager.hpp"

#include <numeric>


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
		, endTimePointValue_( 0 )
		, success_( true )
	{
		std::cout.precision( 3 );
	}

	TesterManager::~TesterManager()
	{
		Shutdown();
	}

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

		futureResults_.reserve( testCount );

		PerformTests( threadsCount );
		ReportDuration();

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path_ << std::endl;
	}


	void TesterManager::PerformTests( size_t noOfThreads )
	{
		size_t index{ 0 };

		threadPool_.Launch( noOfThreads );

		startTimePoint_ = std::chrono::high_resolution_clock::now();

		while ( !tests_.empty() )
		{
			const auto& currentTest = tests_.top();

			const auto future = threadPool_.QueueJob( [this]( auto test ) -> TestResult
				{
					return this->Run( test );
				}, currentTest
			);

			futureResults_.emplace_back( std::move( future ) );

			tests_.pop();
		}
	}

	void TesterManager::RunPerformanceTests() const
	{
		if ( !success_ )
			return;

		auto& test = performance::PerformanceTestManager::Get();
		std::cout << "Now Testing: " << test.GetName() << " ";
		test.Run();
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

		endTimePointValue_.store( endTime.time_since_epoch().count() );
		return result;
	}

	void TesterManager::ReportDuration()
	{
		std::vector<TestResult> results;
		results.reserve( futureResults_.size() );

		for ( auto& fRes : futureResults_ )
		{
			const auto result = fRes.get();
			WriteToFile( result.report );
			results.emplace_back( result );
		}

		futureResults_.clear();

		// Sort fastest -> slowest
		std::ranges::sort( results, []( const TestResult& lhs, const TestResult& rhs )
		{
			return lhs.duration < rhs.duration;
		} );

		for ( const auto& result : results )
		{
			WriteToConsole( result );
		}

		const auto finalTime = std::chrono::duration_cast<TargetDuration_t>( std::chrono::nanoseconds( endTimePointValue_ ) - startTimePoint_.time_since_epoch() ).count();
		const auto millis = finalTime % TargetDuration_t::period::den;
		const auto secs = finalTime - millis;
		const auto avgTime = GetAverageTime( results );

		auto timeStr = Sprintf( "Total Runtime: %us %ums | ", secs, millis );
		timeStr.append( Sprintf( "Average Runtime: %.3fms", avgTime ) );

		WriteToFile( timeStr );

		std::cout << "\n" << timeStr << "\n";
	}

	void TesterManager::WriteToConsole( const TesterManager::TestResult& result )
	{
		const auto& pass = result.passed;
		const auto& name = result.testName;
		const auto millis = static_cast<double>( result.duration.count() ) / 1'000.0;
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

	double TesterManager::GetAverageTime( const std::vector<TestResult>& results ) const
	{
		double avgTime = 0;
		for ( const auto& result : results )
		{
			const auto millis = std::chrono::duration_cast<TargetDuration_t>( result.duration );
			avgTime += static_cast<double>( millis.count() );
		}

		avgTime /= static_cast<double>( results.size() );
		return avgTime;
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
		futureResults_.clear();
	}
}
#endif // TESTING_ENABLED
