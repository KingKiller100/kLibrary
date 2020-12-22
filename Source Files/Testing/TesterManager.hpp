#pragma once

#include "../HelperMacros.hpp"

#include <fstream>
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
		void Run(TesterBase& test);
		void RunAll(bool multiThreaded);
		void ClearAllTests();

		static TesterManager& Get();

	private:
		void WriteToConsole(const bool result, const std::string& NameOpenerStr,
		                           const std::string& resTimeStr);
		double GetAverageTime() const;
		void WriteToFile(const std::string& results);
		
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
