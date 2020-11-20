#pragma once

#include "../../HelperMacros.hpp"
#include "../String/kStringConverter.hpp"

namespace klib::kMisc
{
	template<class Char_t, class Integer_t, class = std::enable_if_t<
		std::is_integral_v<Integer_t>
		>>
		USE_RESULT constexpr std::basic_string_view<Char_t> GetNumberSuffix(Integer_t n)
	{
		if _CONSTEXPR17(std::is_same_v<Char_t, wchar_t>)
		{
			return (n % 10) == 1 ? L"st"
				: (n % 10) == 2 ? L"nd"
				: (n % 10) == 3 ? L"rd"
				: L"th";
		}
		else if _CONSTEXPR17(std::is_same_v<Char_t, char16_t>)
		{
			return (n % 10) == 1 ? u"st"
				: (n % 10) == 2 ? u"nd"
				: (n % 10) == 3 ? u"rd"
				: u"th";
		}
		else if _CONSTEXPR17(std::is_same_v<Char_t, char32_t>)
		{
			return (n % 10) == 1 ? U"st"
				: (n % 10) == 2 ? U"nd"
				: (n % 10) == 3 ? U"rd"
				: U"th";
		}
		else if _CONSTEXPR17(std::is_same_v<Char_t, char8_t>)
		{
			return (n % 10) == 1 ? u8"st"
				: (n % 10) == 2 ? u8"nd"
				: (n % 10) == 3 ? u8"rd"
				: u8"th";
		}
		else
		{
			return (n % 10) == 1 ? "st"
				: (n % 10) == 2 ? "nd"
				: (n % 10) == 3 ? "rd"
				: "th";

		}
	}
}
