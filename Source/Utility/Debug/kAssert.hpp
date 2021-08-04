#pragma once

#include "Source/kSourceInfo.hpp"

#include <exception>
#include <string>
#include <functional>

#if defined(KLIB_DEBUG) || defined(KLIB_TEST)
namespace klib::kDebug
{
	class FailedConditionException final : public std::exception
	{
	public:
		using AssertFunc_t = std::function<void(std::string_view, std::string_view, const SourceInfo&)>;
		
	public:
		FailedConditionException(const std::string_view& expected, const std::string_view& msg
			, const SourceInfo& sourceInfo, const AssertFunc_t& cb);
		~FailedConditionException() throw();
	};

	static FailedConditionException::AssertFunc_t NoAssertCB = nullptr;
}

#	define kAssertCB(condition, msg, cb)\
	{\
		if( (condition) == false )\
			throw ::klib::kDebug::FailedConditionException(#condition, msg, SOURCE_INFO(), cb);\
	}\

#	define kAssert(condition, msg) kAssertCB(condition, msg, klib::kDebug::NoAssertCB)

#else
#	define kAssertCB(condition, msg, cb) ((void)0)
#	define kAssert(condition, msg) ((void)0);
#endif

