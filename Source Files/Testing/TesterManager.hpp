#pragma once

#include "../HelperMacros.hpp"
#include <memory>
#include <set>
#include <string>

#ifdef TESTING_ENABLED
namespace kTest
{
	class Tester;
	
	class TesterManager
	{
		struct Token {};
	public:
		TesterManager(Token&);
		~TesterManager();

		void Initialize();
		void Shutdown();
		void InitializeMathsTests();
		void InitializeUtilityTests();
		void InitializeTemplateTests();
		void RunPerformanceTests() const;
		void Add(Tester* test);
		void Run(Tester& test);
		void RunAll();
		void ClearAllTests();

		static TesterManager& Get();

	private:
		std::string WriteResults(const bool result, const double resTime) const;
		
	private:
		std::string path;
		std::set< std::unique_ptr<Tester> > testsUSet;

		bool success;
	};
}

#endif
