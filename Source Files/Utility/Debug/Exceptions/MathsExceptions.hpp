#pragma once

#include "../../String/kToString.hpp"

#include <stdexcept>


namespace klib::kDebug
{
	class MathsError : public std::logic_error
	{
	public:
		explicit MathsError();
		explicit MathsError(const char* const _Message);
		explicit MathsError(const std::string& _Message);

		~MathsError() noexcept override = default;
	};

	class DivByZeroError final : public MathsError
	{
	public:
		explicit DivByZeroError();
		explicit DivByZeroError(const char* const _Message);
		explicit DivByZeroError(const std::string& _Message);

		~DivByZeroError() noexcept override = default;
	};

	class NoRealRootError final : public MathsError
	{
	public:
		template<typename T>
		NoRealRootError(const T value, const size_t root)
			: MathsError(kString::ToString("ERROR: Value \"{0:2}\" has no {1}{2} root",
				value,
				root,
				(root % 10) == 1 ? "st"
				: (root % 10) == 2 ? "nd"
				: (root % 10) == 3 ? "rd"
				: "th"))
		{}

		explicit NoRealRootError(const char* const _Message);
		explicit NoRealRootError(const std::string& _Message);

		~NoRealRootError() noexcept override;
	};

	class MathsLogicError final : public MathsError
	{
	public:
		explicit MathsLogicError(const char* const _Message);
		explicit MathsLogicError(const std::string& _Message);

		~MathsLogicError() noexcept override;
	};

}
