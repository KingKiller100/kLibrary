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
		: success( true )
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

		path = ( cwd / "Test Results" ).string();
		path += "/";

		if ( !std::filesystem::exists( path ) )
		{
			if ( !std::filesystem::create_directory( path.c_str() ) )
			{
				throw std::runtime_error( "Test results directory could not be created/found. Please check why!" );
			}
		}

		path += "Results.txt";
		std::filesystem::remove( path.c_str() );

		file.open( path, std::ios::out | std::ios::app );
	}

	void TesterManager::Add( TesterBase* test )
	{
		tests.push( std::unique_ptr<TesterBase>( test ) );
	}

	void TesterManager::RunAll( ResourceUtilization resourceUtilization )
	{
		using namespace std::chrono;

		const auto testCount = tests.size();

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

		testResults.resize( testCount );
		high_resolution_clock::time_point startTimePoint, endTimePoint;
		PerformTests( threadsCount, startTimePoint, endTimePoint );
		ReportDuration( startTimePoint, endTimePoint );

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
	}


	void TesterManager::PerformTests(
		size_t noOfThreads
		, std::chrono::high_resolution_clock::time_point& outStartTimePoint
		, std::chrono::high_resolution_clock::time_point& outEndTimePoint
	)
	{
		size_t index{0};

		outStartTimePoint = std::chrono::high_resolution_clock::now();
		if ( noOfThreads > 1 )
		{
			klib::kThread::ThreadPool testPool( noOfThreads );
			while ( !tests.empty() )
			{
				const auto& currentTest = tests.top();
				testPool.QueueJob( kThread::ThreadPool::Job(
					[this, currentTest, i = index++]
					{
						Run( currentTest, i );
					}
					, currentTest->GetName() ) );
				tests.pop();
			}
		}
		else
		{
			while ( !tests.empty() )
			{
				Run( tests.top(), index++ );
				tests.pop();
			}
		}

		outEndTimePoint = std::chrono::high_resolution_clock::now();

		std::sort( testResults.begin(), testResults.end(), []( const TestResult& lhs, const TestResult& rhs )
		{
			return lhs.duration < rhs.duration;
		} );

		for ( const auto& result : testResults )
		{
			WriteToFile( result.report );
			WriteToConsole( result );
		}
	}

	void TesterManager::ReportDuration(
		std::chrono::high_resolution_clock::time_point startTimePoint
		, std::chrono::high_resolution_clock::time_point endTimePoint
	)
	{
		const auto finalTime = std::chrono::duration_cast<std::chrono::milliseconds>( endTimePoint - startTimePoint ).count();
		const auto millis = finalTime % std::chrono::milliseconds::period::den;
		const auto secs = finalTime - millis;
		const auto avgTime = GetAverageTime();

		auto timeStr = Sprintf( "Total Runtime: %us %ums | ", secs, millis );
		timeStr.append( Sprintf( "Average Runtime: %.3fms", avgTime ) );

		WriteToFile( timeStr );

		std::cout << "\n" << timeStr << "\n";
	}

	void TesterManager::RunPerformanceTests() const
	{
		if ( !success )
			return;

		auto& test = performance::PerformanceTestManager::Get();
		std::cout << "Now Testing: " << test.GetName() << " ";
		test.Run();
	}

	// void TesterManager::RunThreaded( std::shared_ptr<TesterBase> test, std::promise<TestResult> promise )
	// {
	// const auto result = Run( std::move( test ) );
	// promise.set_value( result );
	// }

	void TesterManager::Run( std::shared_ptr<TesterBase> test, size_t index )
	{
		TestResult& result = testResults[index];

		const auto start = std::chrono::high_resolution_clock::now();
		const auto passed = test->Run();
		const auto duration = std::chrono::high_resolution_clock::now() - start;

		result.testName = test->GetName();
		result.passed = passed;
		result.duration = std::chrono::duration_cast<std::chrono::microseconds>( duration );

		const auto durationStr =
			Sprintf( "| Runtime: %.3fms", static_cast<double>( duration.count() ) / 1'000'000.0 );

		result.report = passed
			                ? Sprintf( "Success: %s %s\n\n", result.testName, durationStr )                          // Success Case
			                : Sprintf( "Failure: %s %s\n%s", result.testName, durationStr, test->GetFailureData() ); // Fail Case
	}

	void TesterManager::WriteToConsole( const TesterManager::TestResult& result )
	{
		const auto& pass = result.passed;
		const auto& name = result.testName;
		const auto& duration = std::chrono::duration_cast<std::chrono::microseconds>( result.duration );

		auto* const hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

		std::cout << "Ran: " << name << "| ";

		SetConsoleTextAttribute( hConsole, pass
			                                   ? kMisc::ConsoleColour::LIGHT_GREEN
			                                   : kMisc::ConsoleColour::SCARLET_RED );

		std::cout << ( pass ? "Pass" : "Fail" );

		SetConsoleTextAttribute( hConsole, kMisc::ConsoleColour::LIGHT_GREY );

		std::cout << " | " << static_cast<double>( duration.count() ) / 1'000.0 << "ms\n";
	}

	void TesterManager::WriteToFile( std::string_view results )
	{
		file << results;
		file.flush();
	}

	double TesterManager::GetAverageTime() const
	{
		double avgTime = 0;
		for ( const auto& result : testResults )
		{
			const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>( result.duration );
			avgTime += static_cast<double>( millis.count() );
		}

		avgTime /= static_cast<double>( testResults.size() );
		return avgTime;
	}

	void TesterManager::Shutdown()
	{
		if ( file.is_open() )
			file.close();

		ClearAllTests();
	}

	void TesterManager::ClearAllTests()
	{
		testResults.clear();
	}
}
#endif // TESTING_ENABLED
