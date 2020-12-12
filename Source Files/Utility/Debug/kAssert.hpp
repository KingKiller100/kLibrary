#pragma once

#include "Source/SourceInfo.hpp"

#include <exception>
#include <string>
#include <functional>

#if defined(KLIB_DEBUG) || defined(KLIB_TEST)
namespace klib::kDebug
{
	class FailedConditionException final : public std::exception
	{
	public:
		using AssertFunc_t = std::function<void(std::string&, const SourceInfo&)>;
		
	public:
		FailedConditionException(const std::string_view& expected, const std::string_view& msg
			, const char* file, std::int32_t line, const AssertFunc_t& cb);
		~FailedConditionException() throw();

		char const* what() const override;
		
	private:
		std::string report;
	};

	static FailedConditionException::AssertFunc_t NoAssertCB = nullptr;
}

#	define kAssert(condition, msg, cb)\
	{\
		if( (condition) == false )\
			throw ::klib::kDebug::FailedConditionException(#condition, msg, __FILE__, __LINE__, cb);\
	}\

#else
#	define kAssert(condition, msg) ((void)0);
#endif

