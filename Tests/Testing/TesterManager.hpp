#pragma once

#include "EnableTesting.hpp"

#ifdef TESTING_ENABLED
#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

namespace kTest
{
	class TesterBase;

	class TesterManager
	{
	public:
		using Test_t = TesterBase;

		TesterManager();
		~TesterManager();

		void Initialize(bool skipPerformanceTesting = false);
		void Shutdown();
		void RunPerformanceTests() const;
		void Add(TesterBase* test);
		void RunAll( bool threaded = 1 );
		void ClearAllTests();
		
	private:
		void Run(TesterBase& test);
		void WriteToConsole(
			bool pass
			, std::string_view nameOpenerStr
			, std::string_view resTimeStr
		);
		[[nodiscard]] double GetAverageTime() const;
		void WriteToFile( std::string_view results );
		void PerformTests( size_t noOfThreads, clock_t& outStart);

	private:
		std::string path;
		std::set<std::shared_ptr<TesterBase>> tests;
		std::vector<double> testTimes;
		bool success;
		bool skipPerformanceTests;
		std::mutex consoleMutex;
		std::mutex fileMutex;
		std::ofstream file;
	};
}

#endif
