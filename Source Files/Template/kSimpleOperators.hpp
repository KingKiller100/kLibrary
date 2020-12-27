#pragma once
#include "../HelperMacros.hpp"

#include <type_traits>

namespace klib::kTemplate
{
	template<typename T>
	struct SimpleOperators
	{
		template<typename Other_t, class = std::enable_if_t<
			!std::is_same_v<T, Other_t>
		>>
		USE_RESULT friend constexpr bool operator==(const T& obj, Other_t other) noexcept(std::is_arithmetic_v<Other_t>)
		{
			return other == static_cast<Other_t>(obj);
		}
		

		template<typename Other_t, class = std::enable_if_t<
			!std::is_same_v<T, Other_t>
			>>
		USE_RESULT friend constexpr bool operator!=(const T& obj, Other_t other) noexcept(std::is_arithmetic_v<Other_t>)
		{
			return other != static_cast<Other_t>(obj);
		}

		template<typename Other_t, class = std::enable_if_t<
			!std::is_same_v<T, Other_t>
			>>
		USE_RESULT friend constexpr bool operator<(const T& obj, Other_t other) noexcept(std::is_arithmetic_v<Other_t>)
		{
			return other < static_cast<Other_t>(obj);
		}

		template<typename Other_t, class = std::enable_if_t<
			!std::is_same_v<T, Other_t>
			>>
		USE_RESULT friend constexpr bool operator<=(const T& obj, Other_t other) noexcept(std::is_arithmetic_v<Other_t>)
		{
			return other <= static_cast<Other_t>(obj);
		}
		

		template<typename Other_t, class = std::enable_if_t<
			!std::is_same_v<T, Other_t>
			>>
		USE_RESULT friend constexpr bool operator>(const T& obj, Other_t other) noexcept(std::is_arithmetic_v<Other_t>)
		{
			return other > static_cast<Other_t>(obj);
		}

		template<typename Other_t, class = std::enable_if_t<
			!std::is_same_v<T, Other_t>
			>>
		USE_RESULT friend constexpr bool operator>=(const T& obj, Other_t other) noexcept(std::is_arithmetic_v<Other_t>)
		{
			return other >= static_cast<Other_t>(obj);
		}
	};
}
