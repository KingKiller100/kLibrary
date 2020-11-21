#pragma once

#include "../HelperMacros.hpp"
#include <memory>
#include <set>
#include <vector>
#include <string>

#ifdef TESTING_ENABLED
namespace kTest
{
	class Tester;
	
	class TesterManager
	{
		struct Token {};
	public:
		using Test_t = Tester;
		
		TesterManager(Token&);
		~TesterManager();

		void Initialize();
		void InitializeMaths() const;
		void InitializeUtility() const;
		void InitializeTemplates() const;
		void Shutdown();
		void RunPerformanceTests() const;
		void Add(Tester* test);
		void Run(Tester& test);
		void RunAll();
		void ClearAllTests();

		static TesterManager& Get();

	private:
		USE_RESULT std::string WriteResults(const bool result, const double resTime) const;
		
	private:
		std::string path;
		std::set< std::unique_ptr<Tester> > testsSet;
		std::vector<double> timesRecorded;
		bool success;
	};
}

#endif
