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
		enum Threads : size_t
		{
			NONE = 0,
			SINGLE = NONE,
			DUAL = 2,
			TRIPLE = 3,
			QUAD = 4,
			HEX = 6,
			HEPT = 7,
			OCT = 8,
			DEC = 10,
			DODEC = 12,
			SIMPLE_THREAD_RIPPER = 16,
			MEDIUM_THREAD_RIPPER = 32,
			SERIOUS_THREAD_RIPPER = 64,
			ULTRA_THREAD_RIPPER = 128,
		};
		
	public:
		using Test_t = TesterBase;

		TesterManager();
		~TesterManager();

		void Initialize(bool skipPerformanceTesting = false);
		void Shutdown();
		void RunPerformanceTests() const;
		void Add(TesterBase* test);
		void RunAll(size_t noOfThreads = 1);
		void ClearAllTests();
		
	private:
		void Run(TesterBase& test);
		void WriteToConsole(const bool pass, const std::string& nameOpenerStr,
			const std::string& resTimeStr);
		[[nodiscard]] double GetAverageTime() const;
		void WriteToFile(const std::string& results);
		void PerformTests(const size_t noOfThreads, clock_t& outStart);

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
