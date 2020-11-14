#pragma once

#include <stdexcept>
#include <string>


namespace klib::kDebug
{
	class StringError : public std::runtime_error
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
