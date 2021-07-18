#pragma once

#include "EnableTesting.hpp"

#ifdef TESTING_ENABLED

#include "../Utility/String/kSprintf.hpp"

#include "../TypeTraits/Constexpr.hpp"

#include <string>

namespace kTest
{
	class TesterBase : public klib::type_trait::NonCopyable
	{
	protected:
		using TestCaseFunc = std::function<void()>;

	public:
		TesterBase(const char* name) noexcept;
		TesterBase(TesterBase&& other) noexcept;
		TesterBase& operator=(TesterBase&& other) noexcept;

		virtual ~TesterBase();

		[[nodiscard]] const char* GetName() const noexcept;
		[[nodiscard]] const std::string& GetFailureData() const noexcept;

		bool Run() noexcept;

	protected:
		virtual void Prepare() noexcept = 0;
		virtual void CleanUp();
		void AddTestCaseImpl(const char* testName, TestCaseFunc testFunc);

	protected:
		void ReportFailedTestCase(const char* condition, const char* file, const char* function, const std::uint32_t line);
	
	private:
		bool success;

		std::string name;
		std::string failureData;
		std::unordered_map<std::string, TestCaseFunc> testCases;
		std::string_view currentTestName;
	};

#define ADD_TEST(test) this->AddTestCaseImpl(#test, [this](){ test; })
	
	// If results are wrong, change name to failed test function signature and line, else continues to next line
#define VERIFY(test)\
	if ((test) == false)\
	{\
		this->ReportFailedTestCase(#test, __FILE__, __FUNCTION__, __LINE__); \
	}\

#if MSVC_PLATFORM_TOOLSET > 141
	// Verify result of a test if result is available at compile time
#	define VERIFY_COMPILE_TIME(test) klib::type_trait::Compile_Time_Test< test >::value;
#else
#	define VERIFY_COMPILE_TIME(test) VERIFY(test)
#endif

}
#endif
