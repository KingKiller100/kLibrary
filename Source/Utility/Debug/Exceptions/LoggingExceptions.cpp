#include "pch.hpp"

#include "LoggingExceptions.hpp"

namespace klib::kDebug
{
	LoggingExceptions::LoggingExceptions( std::string_view message ): ExceptionBase( message )
	{ }
}
