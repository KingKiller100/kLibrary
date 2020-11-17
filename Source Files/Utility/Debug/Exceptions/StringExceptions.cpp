#include "pch.hpp"
#include "StringExceptions.hpp"

#include <string>

namespace klib::kDebug
{
	StringError::StringError(const std::string_view& msg)
		: runtime_error(msg.data())	
	{
	}

	FormatError::FormatError(const std::string_view& msg)
		: StringError(msg)
	{
	}
}
