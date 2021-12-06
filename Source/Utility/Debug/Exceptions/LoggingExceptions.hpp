#pragma once

#include "ExceptionBase.hpp"

namespace klib::kDebug
{
	class LoggingExceptions : ExceptionBase
	{
	public:
		explicit LoggingExceptions( std::string_view message );
	};
}
