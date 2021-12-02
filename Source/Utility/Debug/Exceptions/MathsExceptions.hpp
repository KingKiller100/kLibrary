#pragma once

#include <string>
#include <stdexcept>

namespace klib::kDebug
{
	class MathsError : public std::logic_error
	{
	public:
		explicit MathsError( std::string_view msg = "Maths Error" );

		~MathsError() noexcept override = default;
	};

	class DivByZeroError final : public MathsError
	{
	public:
		explicit DivByZeroError( std::string_view msg = "ERROR: Division by zero has occurred!" );
	};

	class NoRealRootError final : public MathsError
	{
	public:
		template <typename T>
		NoRealRootError( T value, const size_t root );
		explicit NoRealRootError( std::string_view msg );
	};
}
