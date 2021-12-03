#pragma once

#include "ExceptionBase.hpp"

namespace klib::kDebug
{
	class LoggingExceptions : ExceptionBase
	{
		LoggingExceptions( std::string_view message )
			: ExceptionBase( message )
		{ }
	};

	class LogProfileError
	{
	public:
		LogProfileError();
	};
}
