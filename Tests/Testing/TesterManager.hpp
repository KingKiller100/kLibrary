#pragma once

#include "EnableTesting.hpp"

#ifdef TESTING_ENABLED
#include "../kLib.hpp"
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
		using TargetDuration_t = std::chrono::milliseconds;
		using TargetSubDuration_t = std::chrono::microseconds;
		
		enum class InitializationRequest { All = 0b0 };

		enum class ResourceUtilization { All, Half, Single };

		struct TestResult
		{
			std::string testName;
			std::string report;
			TargetSubDuration_t duration;
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
		TesterManager::TestResult Run( std::shared_ptr<TesterBase> test );
		static void WriteToConsole( const TestResult& result );
		[[nodiscard]] double GetAverageTime( const std::vector<TestResult>& results ) const;
		void WriteToFile( std::string_view results );
		std::vector<std::shared_future<TestResult>> PerformTests( size_t noOfThreads );
		void ReportDuration( std::vector<std::shared_future<TestResult>> futureResults );
	private:
		std::ofstream file_;
		klib::kThread::ThreadPool threadPool_;
		std::string path_;
		std::stack<std::shared_ptr<TesterBase>> tests_;
		std::chrono::high_resolution_clock::time_point startTimePoint_;
		std::atomic_uint64_t endTimePointValue_;
		bool success_;
	};
}

#endif
