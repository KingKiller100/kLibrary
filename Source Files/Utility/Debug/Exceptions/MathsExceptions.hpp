#pragma once

#include "../../String/kSprintf.hpp"
#include "../../String/Format/kStringIdentity.hpp"
#include "../../Misc/kNumberSuffix.hpp"
#include <string>
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
		inline static std::string NoSpecifier = "";
	public:
		template<typename T>
		NoRealRootError(const T value, const size_t root)
			: MathsError(kString::stringify::SprintfWrapper("ERROR: Value \"%s\" has no %u%s root",
				kString::stringify::Identity<char, T>::MakeStr(value, NoSpecifier),
				root,
				kMisc::GetNumberSuffix<char>(root)
			))
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
