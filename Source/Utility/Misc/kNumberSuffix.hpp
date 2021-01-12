#pragma once

#include "../../HelperMacros.hpp"

#include <string>

namespace klib::kMisc
{
	template<class Char_t, class Integer_t, class = std::enable_if_t<
		std::is_integral_v<Integer_t>
		>>
		USE_RESULT constexpr std::basic_string_view<Char_t> GetNumberSuffix(Integer_t n)
	{
		const auto remainder = n % 10;
		const auto forceTH = n > 10 && n < 20;

		if _CONSTEXPR_IF(std::is_same_v<Char_t, wchar_t>)
		{
			if (forceTH)
				return L"th";
			
			return remainder == 1 ? L"st"
				: remainder == 2 ? L"nd"
				: remainder == 3 ? L"rd"
				: L"th";
		}
		else if _CONSTEXPR_IF(std::is_same_v<Char_t, char16_t>)
		{
			if (forceTH)
				return u"th";

			return remainder == 1 ? u"st"
				: remainder == 2 ? u"nd"
				: remainder == 3 ? u"rd"
				: u"th";
		}
		else if _CONSTEXPR_IF(std::is_same_v<Char_t, char32_t>)
		{
			if (forceTH)
				return U"th";

			return remainder == 1 ? U"st"
				: remainder == 2 ? U"nd"
				: remainder == 3 ? U"rd"
				: U"th";
		}
#if __cpp_char8_t
		else if _CONSTEXPR_IF(std::is_same_v<Char_t, char8_t>)
		{
			if (forceTH)
				return u8"th";

			return remainder == 1 ? u8"st"
				: remainder == 2 ? u8"nd"
				: remainder == 3 ? u8"rd"
				: u8"th";
		}
#endif
		else
		{
			if (forceTH)
				return "th";

			return remainder == 1 ? "st"
				: remainder == 2 ? "nd"
				: remainder == 3 ? "rd"
				: "th";
		}
	}

}
