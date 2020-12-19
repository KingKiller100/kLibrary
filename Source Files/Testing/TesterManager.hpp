#pragma once

#include "../HelperMacros.hpp"
#include <deque>
#include <memory>
#include <mutex>
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
		void RunAll();
		void ClearAllTests();

		static TesterManager& Get();

	private:
		USE_RESULT std::string WriteResults(const bool result, const double resTime);
		
	private:
		std::string path;
		std::deque<std::shared_ptr<TesterBase>> tests;
		std::vector<double> timesRecorded;
		bool success;
		std::mutex outputLock;
	};
}

#endif
