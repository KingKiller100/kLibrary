#pragma once


#include "EnableTesting.hpp"

#ifdef TESTING_ENABLED
#include <chrono>
#include <fstream>
#include <stack>
#include <memory>
#include <string>
#include <vector>

namespace kTest
{
	class TesterBase;

	class TesterManager
	{
	public:
		enum class InitializationRequest { All = 0b0 };

		enum class ResourceUtilization { All, Half, Single };

		struct TestResult
		{
			std::string testName;
			std::string report;
			std::chrono::microseconds duration;
			bool passed;

			TestResult()
				: testName()
				, report()
				, duration( 0 )
				, passed( true )
			{ }
		};

	public:
		using Test_t = TesterBase;

		TesterManager();
		~TesterManager();

		void Initialize( InitializationRequest initRequest = InitializationRequest::All );
		void Shutdown();
		void RunPerformanceTests() const;
		void Add( TesterBase* test );
		void RunAll( ResourceUtilization resourceUtilization = ResourceUtilization::All );
		void ClearAllTests();

	private:
		void Run( std::shared_ptr<TesterBase> test, size_t index );
		static void WriteToConsole( const TestResult& result );
		[[nodiscard]] double GetAverageTime() const;
		void WriteToFile( std::string_view results );
		void PerformTests( size_t noOfThreads, std::chrono::high_resolution_clock::time_point&, std::chrono::high_resolution_clock::time_point& );
		void ReportDuration(
			std::chrono::high_resolution_clock::time_point startTimePoint
			, std::chrono::high_resolution_clock::time_point endTimePoint
		);
	private:
		std::string path;
		std::stack<std::shared_ptr<TesterBase>> tests;
		std::vector<TestResult> testResults;
		std::ofstream file;
		bool success;
	};
}

#endif
