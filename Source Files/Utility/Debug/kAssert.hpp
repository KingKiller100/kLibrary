#pragma once

#include <exception>
#include <string>
#include <functional>

#if defined(KLIB_DEBUG) || defined(KLIB_TEST)
namespace klib::kDebug
{
	class FailedConditionException final : public std::exception
	{
	public:
		using AssertFunc = std::function<void(std::string&, const std::string_view&, std::int32_t)>;
		
	public:
		FailedConditionException(const std::string_view& expected, const std::string_view& msg
			, const char* file, const std::int32_t line
		, const AssertFunc& cb);
		~FailedConditionException() throw();

		char const* what() const override;
		
	private:
		std::string report;
	};

	static FailedConditionException::AssertFunc NoAssertCB = nullptr;
}

#	define kAssert(condition, msg, cb)\
	{\
		if( (condition) == false )\
			throw ::klib::kDebug::FailedConditionException(#condition, msg, __FILE__, (unsigned)(__LINE__), cb);\
	}\

#else
#	define kAssert(condition, msg) ((void)0);
#endif

