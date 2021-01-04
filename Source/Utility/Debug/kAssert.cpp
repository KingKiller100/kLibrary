#include "pch.hpp"
#include "kAssert.hpp"

#include "../String/kToString.hpp"

#if defined(KLIB_DEBUG) ||  defined(KLIB_TEST)
namespace klib::kDebug
{	
	FailedConditionException::FailedConditionException(const std::string_view& expected,
		const std::string_view& msg, const SourceInfo& sourceInfo, const AssertFunc_t& cb)
		: report(kString::ToString("Condition \"{0}\" was not met! \n               [DETAILS]: {1}.", expected, msg))
	{
		if (cb != nullptr)
			cb(report, sourceInfo);
	}

	FailedConditionException::~FailedConditionException() throw()
	{}

	char const* FailedConditionException::what() const
	{
		return report.c_str();
	}
}
#endif
