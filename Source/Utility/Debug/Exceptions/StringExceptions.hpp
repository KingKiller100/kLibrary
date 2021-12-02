#pragma once

#include "ExceptionBase.hpp"

namespace klib::kDebug
{
	class StringError : public ExceptionBase
	{
	public:
		explicit StringError(const std::string_view& msg);
	};

	class FormatError : public StringError
	{
	public:
		explicit FormatError(const std::string_view& msg);
	};
	
}
