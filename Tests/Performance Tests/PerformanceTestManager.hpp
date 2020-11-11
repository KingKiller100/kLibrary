#pragma once


#include "../../Source Files/Testing/Tester.hpp"

#include <filesystem>
#include <string>
#include <unordered_set>


#ifdef TESTING_ENABLED
namespace kTest::performance
{
	class PerformanceTestBase;

	class PerformanceTestManager : public Tester
	{
		struct Token {};
	public:
		PerformanceTestManager(const Token&);
		~PerformanceTestManager();

		void Add(PerformanceTestBase* test);
		void ClearAll();
		
		void CollectResult(const std::string_view& result);
		
		static PerformanceTestManager& Get();
	
	private:
		void Initialize();
		void ShutDown();
		void RunAll();
		void RunTest(PerformanceTestBase* test);

		void InitializeMaths();

		void Test() override;
		void OutputResult(const std::string& name);

	private:
		std::unordered_set<PerformanceTestBase*> tests;
		std::string results;
		std::filesystem::path path;
	};
}
#endif

