#pragma once

#include "../HelperMacros.hpp"

#include <fstream>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <vector>

#ifdef TESTING_ENABLED
namespace kTest
{
	class TesterBase;

	class TesterManager
	{
		struct Token {};
	public:
		using Test_t = TesterBase;

		TesterManager(Token&);
		~TesterManager();

		void Initialize();
		void InitializeMaths() const;
		void InitializeUtility() const;
		void InitializeTemplates() const;
		void Shutdown();
		void RunPerformanceTests() const;
		void Add(TesterBase* test);
		void RunAll(const size_t noOfThreads);
		void ClearAllTests();

		static TesterManager& Get();

	private:
		void Run(TesterBase& test);
		void WriteToConsole(const bool result, const std::string& nameOpenerStr,
			const std::string& resTimeStr);
		double GetAverageTime() const;
		void WriteToFile(const std::string& results);
		void PerformTests(const size_t noOfThreads, clock_t& start);

	private:
		std::string path;
		std::set<std::shared_ptr<TesterBase>> tests;
		std::vector<double> timesRecorded;
		bool success;
		std::mutex consoleMutex;
		std::mutex fileMutex;
		std::ofstream file;
	};
}

#endif
