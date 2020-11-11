#pragma once

#include <charconv>
#include <stdexcept>
#include <typeinfo>

#include "kSprintf.hpp"
#include "kStringifyHelper.hpp"
#include "kStringifyInteger.hpp"
#include "../kStringConverter.hpp"

namespace klib::kString::stringify
{
	template<class CharType, typename T
		, typename = std::enable_if_t<std::is_floating_point_v<T>
		|| type_trait::Is_CharType_V<CharType>>
		>
		kString::StringWriter<CharType> StringFloatingPoint(const T val, size_t precision, std::chars_format fmt = std::chars_format::fixed)
	{
		if (precision == nPrecision)
			precision = 6;
		
#if defined(_HAS_COMPLETE_CHARCONV) && (_HAS_COMPLETE_CHARCONV == FALSE)
		const std::string format = "%." + StringIntegral<char>(precision, 1) + "f";
		const auto temp = stringify::SprintfWrapper(format, val);
		const auto conv = kString::Convert<CharType>(temp);
		const StringWriter<CharType> str(conv);
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
			throw std::runtime_error(err);
		}

		const StringWriter<CharType> str(kString::Convert<CharType>(buff));
#endif 

		return str;
	}

}