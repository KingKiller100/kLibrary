#include "pch.hpp"
#include "TesterBase.hpp"

#include <iostream>

#ifdef TESTING_ENABLED

namespace kTest
{
	TesterBase::TesterBase(const char* name) noexcept
		: success(true)
		, name(name)
	{	}

	TesterBase::TesterBase(TesterBase&& other) noexcept
		= default;

	TesterBase& TesterBase::operator=(TesterBase&& other) noexcept
		= default;

	TesterBase::~TesterBase()
		= default;

	const char* TesterBase::GetName() const noexcept
	{
		return name.c_str();
	}

	const std::string& TesterBase::GetFailureData() const noexcept
	{
		return failureData;
	}

	bool TesterBase::Run() noexcept
	{
		Prepare();
		
		try
		{
			for (auto&& testData : testCases)
			{
				currentTestName = testData.first;
				const auto& test = testData.second;
				test();
			}
			
			for (auto&& testData : deprecatedTestCases)
			{
				currentTestName = testData.first;
				const auto& test = testData.second;
				(void)test();
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what();
			failureData.append(klib::kString::Sprintf("\tCase \"%s\" crashed. Report: %s\n"
				, currentTestName
				, e.what()));
			success = false;
		}
		currentTestName = "";
		return success;
	}

	void TesterBase::AddTest(const char* testName, TestCaseFunc testFunc)
	{
		testCases.emplace(testName, testFunc);
	}

	void TesterBase::AddTest(const char* testName, DeprecatedTestCaseFunc testFunc)
	{
		deprecatedTestCases.emplace(testName, testFunc);
	}

	void TesterBase::ReportFailedTestCase(const char* condition, const char* file,
	                                      const char* function, const std::uint32_t line)
	{
		success = false;
		failureData.append(klib::kString::Sprintf("\t Test Case: \"%s\" Condition: \"%s\"" 
			"\n\tFile: %s"
			"\n\tFunction: %s"
			"\n\tLine: %d\n\n", 
			currentTestName
			, condition
			, file, function, line));
	}
}

#endif