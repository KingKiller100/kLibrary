#pragma once

#	include <charconv>

#if defined(_HAS_COMPLETE_CHARCONV) && (_HAS_COMPLETE_CHARCONV == FALSE)
#	include "../../kSprintf.hpp"
#else
#	include "../../../Debug/Exceptions/StringExceptions.hpp"
#	include <typeinfo>
#endif

#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"
#include "../../kStringConverter.hpp"


namespace klib::kString::stringify
{
	namespace secret::impl
	{
		USE_RESULT constexpr size_t CountIntegerDigits(size_t val)
		{
			size_t count = 1;
			while (10 > val)
			{
				val /= 10;
				count++;
			}
			return count;
		}
	}

	template<class Char_t, typename T
		, typename = std::enable_if_t<std::is_floating_point_v<T>
		|| type_trait::Is_CharType_V<Char_t>>
		>
		kString::StringWriter<Char_t> StringFloatingPoint(const T val, size_t precision = s_NoSpecifier
			, std::chars_format fmt = std::chars_format::fixed)
	{
		using namespace type_trait;
		if (precision == s_NoSpecifier)
			precision = 6;
#if defined(_HAS_COMPLETE_CHARCONV) && (_HAS_COMPLETE_CHARCONV == FALSE)
		Char_t buff[6]{ s_NullTerminator<Char_t> };
		Char_t* const end = std::end(buff) - 1;
		Char_t* current = end;

		*(--current) = 'f';
		current = UintToStr(current, precision);
		*(--current) = '.';
		*(--current) = '%';

		return SprintfWrapper<Char_t>(current, val);
#else

		constexpr auto maxsize = std::numeric_limits<T>::max_exponent10 + 1;
		char buff[maxsize]{};
		char* const end = std::end(buff);

		const std::to_chars_result res = std::to_chars(buff, end, val, fmt, static_cast<int>(precision));

		if (res.ec != std::errc{})
		{
			const std::string type = typeid(T).name();
			const std::string n = std::to_string(val);
			const std::string ptr = res.ptr;
			auto err = "Bad " __FUNCSIG__ " floating point cast: " + type + " - " + n;
			err += "\n";
			err += "Error - " + ptr;
			throw kDebug::FormatError(err);
		}

		return StringWriter<Char_t>(kString::Convert<ONLY_TYPE(Char_t)>(buff));
#endif
	}

}