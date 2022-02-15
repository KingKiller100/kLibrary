#include "TesterManager.hpp"

#include <chrono>
#include <chrono>
#include <numeric>

#ifdef TESTING_ENABLED

#include "TesterBase.hpp"

#include "SetUpTests.hpp"

// Speed Testing
#include "Performance/PerformanceTestManager.hpp"

#include "../kLib.hpp"

#include <iostream>
#include <stdexcept>
#include <mutex>
#include <utility>
#include <Windows.h>

namespace kTest
{
	using namespace klib;
	using namespace kString;

	TesterManager::TesterManager()
		: success( true )
		, skipPerformanceTests( false )
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

		skipPerformanceTests = ( initRequest == InitializationRequest::NoPerformanceTests );

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

		const auto coresToUse = resourceUtilization == ResourceUtilization::All
			                        ? maxResources
			                        : resourceUtilization == ResourceUtilization::Half
			                        ? maxResources >> 1
			                        : 1;

		const auto threadsCount = ( std::min<size_t> )( coresToUse, testCount );

		std::cout << "Testing: " <<
			( threadsCount > 1 ? "Multi-Threaded" : "Single Threaded" ) <<
			"[" << threadsCount << "]" << "\n";

		testResults.reserve( testCount );
		high_resolution_clock::time_point startTimePoint;
		PerformTests( threadsCount, startTimePoint );
		const auto endTimePoint = high_resolution_clock::now();

		std::sort( testResults.begin(), testResults.end(), []( const TestResult& lhs, const TestResult& rhs )
		{
			return lhs.duration < rhs.duration;
		} );

		for ( const auto& result : testResults )
		{
			WriteToFile( result.report );
			WriteToConsole( result );
		}

		const auto finalTime = duration_cast<milliseconds>( endTimePoint - startTimePoint ).count();
		const auto millis = finalTime % milliseconds::period::den;
		const auto secs = finalTime - millis;
		const auto avgTime = GetAverageTime();

		auto timeStr = Sprintf( "Total Runtime: %us %ums | ", secs, millis );
		timeStr.append( Sprintf( "Average Runtime: %.3fms", avgTime ) );

		WriteToFile( timeStr );

		std::cout << "\n" << timeStr << "\n";

		std::cout << "\nTests have concluded. Please find results in the following path:\n" << path << std::endl;
	}

	void TesterManager::PerformTests( size_t noOfThreads, std::chrono::high_resolution_clock::time_point& outStart )
	{
		if ( noOfThreads > 1 )
		{
			std::vector<std::future<TestResult>> testFutures;

			outStart = std::chrono::high_resolution_clock::now();
			while ( !tests.empty() )
			{
				const auto loops = ( std::min )( noOfThreads, tests.size() );
				for ( size_t i = 0; i < loops; ++i )
				{
					auto test = tests.top();
					testFutures.emplace_back(  std::async( std::launch::async, &TesterManager::Run, test ) );
					tests.pop();
				}
			}

			for ( auto& future : testFutures )
			{
				testResults.emplace_back( future.get() );
			}
		}
		else
		{
			outStart = std::chrono::high_resolution_clock::now();
			while ( !tests.empty() )
			{
				testResults.emplace_back( Run( tests.top() ) );
				tests.pop();
			}
		}
	}

	void TesterManager::RunPerformanceTests() const
	{
		if ( !success || skipPerformanceTests )
			return;

		auto& test = performance::PerformanceTestManager::Get();
		std::cout << "Now Testing: " << test.GetName() << " ";
		test.Run();
	}

	void TesterManager::RunThreaded( std::shared_ptr<TesterBase> test, std::promise<TestResult> promise )
	{
		const auto result = Run( std::move( test ) );
		promise.set_value( result );
	}

	TesterManager::TestResult TesterManager::Run( std::shared_ptr<TesterBase> test )
	{
		TestResult result;

		const auto start = std::chrono::high_resolution_clock::now();
		const auto passed = test->Run();
		const auto duration = std::chrono::high_resolution_clock::now() - start;

		result.testName = test->GetName();
		result.passed = passed;
		result.duration = std::chrono::duration_cast<std::chrono::microseconds>( duration );

		const auto durationStr =
			Sprintf( "| Runtime: %.3fms", static_cast<double>( duration.count() ) / 1'000'000 );

		result.report = passed
			                ? Sprintf( "Success: %s %s\n\n", result.testName, durationStr )                          // Success Case
			                : Sprintf( "Failure: %s %s\n%s", result.testName, durationStr, test->GetFailureData() ); // Fail Case

		return result;
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
