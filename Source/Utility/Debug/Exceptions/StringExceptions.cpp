#include "pch.hpp"
#include "StringExceptions.hpp"

namespace klib::kDebug
{
	StringError::StringError( const std::string_view& msg )
		: ExceptionBase( msg )
	{ }

	FormatError::FormatError( const std::string_view& msg )
		: StringError( msg )
	{ }
}
