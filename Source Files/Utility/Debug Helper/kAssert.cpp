#include "pch.hpp"
#include "kAssert.hpp"

#include "../String/kToString.hpp"

#if defined(KLIB_DEBUG) ||  defined(KLIB_TEST)
namespace klib::kDebug
{	
	FailedConditionException::FailedConditionException(const std::string_view& expected,
		const std::string_view& msg, const char* file, const std::int32_t line, const AssertFunc& cb)
		: report(kString::ToString("Condition \"{0}\" was not met! \n               [DETAILS]: {1}.", expected, msg))
	{
		if (cb != nullptr)
			cb(report
				, std::string_view(file)
				, line);
	}

	FailedConditionException::~FailedConditionException() throw()
	{}

	char const* FailedConditionException::what() const
	{
		return report.c_str();
	}
}
#endif
