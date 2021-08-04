#include "pch.hpp"
#include "kAssert.hpp"

#include "../String/kToString.hpp"

#if defined(KLIB_DEBUG) ||  defined(KLIB_TEST)
namespace klib::kDebug
{	
	FailedConditionException::FailedConditionException(const std::string_view& expected,
		const std::string_view& msg, const SourceInfo& sourceInfo, const AssertFunc_t& cb)
	{
		if (cb != nullptr)
			cb(expected, msg, sourceInfo);
	}

	FailedConditionException::~FailedConditionException() throw()
	{}
}
#endif
