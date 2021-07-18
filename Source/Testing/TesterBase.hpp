#pragma once

#include "../HelperMacros.hpp"

#include "../Utility/String/kSprintf.hpp"

#include "../TypeTraits/Constexpr.hpp"

#include <string>

#ifdef TESTING_ENABLED
namespace kTest
{
	class TesterBase : public klib::type_trait::NonCopyable
	{
	protected:
		using TestCaseFunc = std::function<void()>;
		using DeprecatedTestCaseFunc = std::function<bool()>;

	public:
		TesterBase(const char* name) noexcept;
		TesterBase(TesterBase&& other) noexcept;
		TesterBase& operator=(TesterBase&& other) noexcept;

		virtual ~TesterBase();

		const char* GetName() const noexcept;
		const std::string& GetFailureData() const noexcept;

		bool Run() noexcept;

	protected:
		virtual void Prepare() noexcept = 0;
		void AddTest(const char* testName, TestCaseFunc testFunc);
		void AddTest(const char* testName, DeprecatedTestCaseFunc testFunc);

	protected:
		void ReportFailedTestCase(const char* condition, const char* file, const char* function, const std::uint32_t line);
	
	private:
		bool success;

		std::string name;
		std::string failureData;
		std::unordered_map<std::string, TestCaseFunc> testCases;
		std::unordered_map<std::string, DeprecatedTestCaseFunc> deprecatedTestCases;
		std::string_view currentTestName;
	};

#define ADD_TEST(test) this->AddTest(#test, [this](){ test; });
	
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
