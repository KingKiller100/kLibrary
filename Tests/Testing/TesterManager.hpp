#pragma once


#include "EnableTesting.hpp"

#ifdef TESTING_ENABLED
#include <fstream>
#include <stack>
#include <memory>
#include <string>
#include <vector>
#include <future>

namespace kTest
{
	class TesterBase;

	class TesterManager
	{
	public:
		enum class InitializationRequest { All = 0b0, NoPerformanceTests = 0b1 };

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
		static void RunThreaded( std::shared_ptr<TesterBase> test, std::promise<TestResult> promise );
		static TestResult Run( std::shared_ptr<TesterBase> test );
		static void WriteToConsole(const TestResult& result);
		[[nodiscard]] double GetAverageTime() const;
		void WriteToFile( std::string_view results );
		void PerformTests( size_t noOfThreads, std::chrono::high_resolution_clock::time_point& outStart );

	private:
		std::string path;
		std::stack<std::shared_ptr<TesterBase>> tests;
		std::vector<TestResult> testResults;
		std::ofstream file;
		bool success;
		bool skipPerformanceTests;
	};
}

#endif
