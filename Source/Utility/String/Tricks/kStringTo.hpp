#pragma once

#include "kStringSize.hpp"
#include "kStringFind.hpp"
#include "kStringDigitChar.hpp"
#include "kStringRemove.hpp"

#include "../kStringTypes.hpp"
#include "../../../Maths/kMathsCount.hpp"
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../Debug/Exceptions/StringExceptions.hpp"

namespace klib::kString
{
	template<class Arithmetic_t, class Char_t, class = std::enable_if_t<
		type_trait::Is_Char_V<Char_t>
		&& std::is_arithmetic_v<Arithmetic_t>
		>>
		USE_RESULT constexpr Arithmetic_t CStrTo(const Char_t* const str, const Char_t* end = nullptr)
	{
		Arithmetic_t result = 0;

		const auto isNeg = std::is_signed_v<Arithmetic_t>
			&& str[0] == Char_t('-');

		if (end == nullptr)
			end = str + GetSize(str);

		const size_t size = end - str;

		if constexpr (std::is_integral_v<Arithmetic_t>)
		{
			static const auto CrashFunc = [](const std::string& errMsg)
			{
				throw kDebug::StringError(errMsg + '\n');
			};

			size_t currentPos = isNeg ? 1 : 0;
			auto magnitude = static_cast<size_t>(std::pow(10, size - 1));
			while (str + currentPos != end)
			{
				if (!IsDigit(str[currentPos]))
				{
					const auto input = ToWriter(Convert<char>(str));
					CrashFunc(__FUNCTION__ "'s string must only contain digits. string: " + input);
				}

				const auto digit = static_cast<std::make_unsigned_t<Arithmetic_t>>(str[currentPos] - Char_t('0'));
				const auto asInt = digit * magnitude;

				result += static_cast<Arithmetic_t>(asInt);
				magnitude /= 10;
				++currentPos;
			}

			if (isNeg)
				result *= -1;

			return result;
		}
		else // Assume it is a floating point
		{
			auto decimalPos = Find(str, Char_t('.'));

			if (decimalPos >= size)
			{
				result = static_cast<Arithmetic_t>(CStrTo<long long>(str, end));
				return result;
			}

			auto integerEnd = str + decimalPos;
			result = static_cast<Arithmetic_t>(CStrTo<long long>(str, integerEnd));

			++decimalPos;

			auto remaining = size - decimalPos;

			auto decimalStart = str + decimalPos;
			auto decimalEnd = decimalStart + remaining;

			auto decimals = static_cast<Arithmetic_t>(CStrTo<long long>(decimalStart, decimalEnd));

			decimals /= std::pow(static_cast<Arithmetic_t>(10), static_cast<Arithmetic_t>(remaining));
			result += decimals;
			return result;
		}
	}

	template<class Arithmetic_t, class Char_t, class = std::enable_if_t<
		type_trait::Is_Char_V<Char_t>
		>>
		USE_RESULT constexpr Arithmetic_t CStrTo(const Char_t* const cstr, Arithmetic_t defaultValue, const Char_t* const end = nullptr)
	{
		const auto val = CStrTo<Arithmetic_t>(cstr, end);
		if (val == 0)
			return defaultValue;
		return val;
	}

	template<class Arithmetic_t, class Char_t, class = std::enable_if_t<
		type_trait::Is_Char_V<Char_t>
		>>
		USE_RESULT constexpr Arithmetic_t TryCStrTo(const Char_t* const cstr, Arithmetic_t defaultValue, const Char_t* const end = nullptr)
	{
		try
		{
			return CStrTo<Arithmetic_t>(cstr, end);
		}
		catch (...)
		{
			return defaultValue;
		}
	}

	// Converts strings containing text for an integer value into the integer type with that value 
	template<class Arithmetic_t, typename StringT
		, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<StringT, std::basic_string>
		>>
		USE_RESULT constexpr Arithmetic_t StrTo(StringT string, size_t count = type_trait::g_NoPos<StringT>)
	{
		using CharType = typename StringT::value_type;

		// Remove(string, CharType(' '));
		// Remove(string, CharType(','));
		// Remove(string, CharType('\''));

		// if (string.empty())
			// return static_cast<Arithmetic_t>(0);

		const auto data = string.data();

		const auto end = count == type_trait::g_NoPos<StringT>
			? nullptr
			: data + count;

		const auto result = CStrTo<Arithmetic_t>(data, end);

		return result;
	}

	// Converts strings containing text for an integer value into the integer type with that value.
	// But if zero results are unwanted, a default value can be returned instead
	template<class Arithmetic_t, typename StringT
		, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<StringT, std::basic_string>
		>>
		USE_RESULT constexpr Arithmetic_t StrTo(StringT string, Arithmetic_t defaultValue, size_t count = type_trait::g_NoPos<StringT>)
	{
		const auto val = StrTo<Arithmetic_t>(string, count);
		if (val == 0)
			return defaultValue;
		return val;
	}

	// Converts strings containing text for an integer value into the integer type with that value.
	// But bad string inputs are likely, ignore the exception and return a default value
	template<class Arithmetic_t, typename StringT
		, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<StringT, std::basic_string>
		>>
		USE_RESULT constexpr Arithmetic_t TryStrTo(StringT string, Arithmetic_t defaultValue, size_t count = type_trait::g_NoPos<StringT>)
	{
		try
		{
			return StrTo<Arithmetic_t>(string, count);
		}
		catch (...)
		{
			return defaultValue;
		}
	}
}
