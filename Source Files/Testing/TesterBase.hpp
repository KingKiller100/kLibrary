#pragma once

#include "../HelperMacros.hpp"

#include "../Utility/String/kSprintf.hpp"

#include "../TypeTraits/Constexpr.hpp"

#include <string>

#ifdef TESTING_ENABLED
namespace kTest
{
	class TesterBase
	{
	public:
		TesterBase(const char* name) noexcept;
		TesterBase(TesterBase&& other) noexcept;
		TesterBase& operator=(TesterBase&& other) noexcept;

		virtual ~TesterBase();

		const char* GetName() const noexcept;
		const std::string& GetFailureData() const noexcept;

		bool Run() noexcept;

		// Deleted Funcs
		TesterBase(const TesterBase& other) = delete;
		TesterBase& operator=(const TesterBase& other) = delete;

	protected:
		virtual void Test() = 0;

	protected:
		bool success;

		std::string name;
		std::string failureData;
	};

	// If results are wrong, change name to failed test function signature and line, else continues to next line
#define VERIFY(test)\
	if ((test) == false)\
	{\
		this->success = false; \
		this->failureData.append(klib::kString::Sprintf("\tCondition: %s\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", #test, __FILE__, __FUNCSIG__, __LINE__));\
	}\

#if MSVC_PLATFORM_TOOLSET > 141
	// Verify result of a test if result is available at compile time
#	define VERIFY_COMPILE_TIME(test) this->success = klib::type_trait::Compile_Time_Test< test >::value;
#else
#	define VERIFY_COMPILE_TIME(test) VERIFY(test)
#endif

	
	// Verify using multiple test functions
#define VERIFY_MULTI_INIT() bool noFails = true;
#define VERIFY_MULTI_END()  this->success = noFails;
#define VERIFY_MULTI(func) VERIFY(func == true)\
if (!success) noFails = false;\
success = true;\


}
#endif
