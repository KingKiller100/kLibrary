#pragma once

#include "kStringSize.hpp"
#include "kStringLocale.hpp"
#include "kStringRemove.hpp"

#include "../kStringTypes.hpp"
#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../Debug/Exceptions/StringExceptions.hpp"


namespace klib::kString
{
	template<class Integer_t, class Char_t, class = std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		>>
		USE_RESULT constexpr Integer_t CStrTo(const Char_t* const str)
	{
		static_assert(std::is_integral_v<Integer_t>, __FUNCTION__ " can only be used with integer types "
			"(char, int, long, unsigned short, etc..");

		const auto CrashFunc = [](const std::string& errMsg) { throw kDebug::StringError(errMsg + '\n'); };
		const auto MaxDigitsFunc = []()
		{
			auto maxNum = std::numeric_limits<Integer_t>::max();
			size_t count = 0;
			do {
				++count;
				maxNum /= 10;
			} while (maxNum);
			return count;
		};

		if (str == nullptr
			|| str[0] == type_trait::g_NullTerminator<Char_t>)
			return static_cast<Integer_t>(0);

		const auto isNeg = std::is_signed_v<Integer_t>
			&& str[0] == Char_t('-');

		Integer_t result = 0;
		auto currentPos = isNeg ? 1 : 0;
		size_t size = isNeg ? GetSize(str) - 1 : GetSize(str);
		auto magnitude = static_cast<size_t>(std::pow(10, size - 1));

		if (size > MaxDigitsFunc())
		{
			const std::string type = typeid(Integer_t).name();
			const auto msg = __FUNCTION__ "'s string contains more digits than largest number of type: "
				+ type;
			CrashFunc(msg);
		}

		while (str[currentPos] != type_trait::g_NullTerminator<Char_t>)
		{
			if (!IsDigit(str[currentPos]))
			{
				const auto input = ToWriter(Convert<char>(str));
				CrashFunc(__FUNCTION__ "'s string must only contain digits. string: " + input);
			}

			const auto digit = static_cast<std::make_unsigned_t<Integer_t>>(str[currentPos] - Char_t('0'));
			const auto asInt = digit * magnitude;

			result += static_cast<Integer_t>(asInt);
			magnitude /= 10;
			++currentPos;
		}

		if (isNeg)
			result *= -1;

		return result;
	}
	
	template<class Integer_t, class Char_t, class = std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		>>
		USE_RESULT constexpr Integer_t CStrTo(const Char_t* const cstr, Integer_t defaultValue)
	{
		const auto val = CStrTo<Integer_t>(cstr);
		if (val == 0)
			return defaultValue;
		return val;
	}
	
	template<class Integer_t, class Char_t, class = std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		>>
		USE_RESULT constexpr Integer_t TryCStrTo(const Char_t* const cstr, Integer_t defaultValue)
	{
		try
		{
			return CStrTo<Integer_t>(cstr);
		}
		catch (...)
		{
			return defaultValue;
		}
	}

	// Converts strings containing text for an integer value into the integer type with that value 
	template<class Integer_t, typename StringT
		, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<StringT, std::basic_string>
		>>
		USE_RESULT constexpr Integer_t StrTo(StringT string)
	{
		using CharType = typename StringT::value_type;

		const auto CrashFunc = [](const std::string& errMsg) { throw kDebug::StringError(errMsg + "\n"); };

		Remove(string, CharType(' '));
		Remove(string, CharType(','));
		Remove(string, CharType('\''));

		if (string.empty())
			return static_cast<Integer_t>(0);

		if (Contains(string, CharType('.')))
			CrashFunc("string must contain only one integer number");

		const auto result = CStrTo<Integer_t>(string.data());

		return result;
	}

	// Converts strings containing text for an integer value into the integer type with that value.
	// But if zero results are unwanted, a default value can be returned instead
	template<class Integer_t, typename StringT
		, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<StringT, std::basic_string>
		>>
		USE_RESULT constexpr Integer_t StrTo(StringT string, Integer_t defaultValue)
	{
		const auto val = StrTo<Integer_t>(string);
		if (val == 0)
			return defaultValue;
		return val;
	}

	// Converts strings containing text for an integer value into the integer type with that value.
	// But bad string inputs are likely, ignore the exception and return a default value
	template<class Integer_t, typename StringT
		, typename = std::enable_if_t<
		type_trait::Is_Specialization_V<StringT, std::basic_string>
		>>
		USE_RESULT constexpr Integer_t TryStrTo(StringT string, Integer_t defaultValue)
	{
		try
		{
			return StrTo<Integer_t>(string);
		}
		catch (...)
		{
			return defaultValue;
		}
	}
}
