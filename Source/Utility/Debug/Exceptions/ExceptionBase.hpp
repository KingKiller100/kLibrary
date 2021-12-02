#pragma once

#include <stdexcept>

namespace klib::kDebug
{
	class ExceptionBase : public std::runtime_error
	{
	protected:
		[[nodiscard]] explicit ExceptionBase( std::string_view message )
			: runtime_error( std::string{message} )
		{}

		~ExceptionBase() override = default;
	};
}
