#pragma once

#include "../TesterBase.hpp"

#ifdef TESTING_ENABLED

#include <filesystem>
#include <string>
#include <unordered_set>

namespace kTest::performance
{
	class PerformanceTestBase;

	class PerformanceTestManager : public TesterBase
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
		void Prepare() noexcept override;
		void CleanUp() override;
	private:
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

