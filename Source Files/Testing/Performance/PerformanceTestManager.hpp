#pragma once

#include "../Tester.hpp"

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
		using Test_t = PerformanceTestBase;
		
		PerformanceTestManager(const Token&);
		~PerformanceTestManager();

		void Add(PerformanceTestBase* test);
		void ClearAll();
		
		void CollectResult(const std::string_view& result);
		
		static PerformanceTestManager& Get();
	
	protected:
		void Test() override;
		
	private:
		void Initialize();
		void ShutDown();
		void RunAll();
		void RunTest(PerformanceTestBase* test);

		void InitializeTests();

		void OutputResult(const std::string& name);

	private:
		std::unordered_set<PerformanceTestBase*> tests;
		std::string results;
		std::filesystem::path path;
	};
}
#endif

