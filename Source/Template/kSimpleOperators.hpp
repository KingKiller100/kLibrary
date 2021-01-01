#pragma once
#include "../HelperMacros.hpp"

#include <type_traits>

namespace klib::kTemplate
{
	template<typename T>
	struct SimpleOperators
	{
		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
		>>
		USE_RESULT friend constexpr bool operator==(const T& obj, const U& other) noexcept(std::is_arithmetic_v<U>)
		{
			return other == static_cast<U>(obj);
		}
		
		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
		>>
		USE_RESULT friend constexpr bool operator==(const U& other, const T& obj) noexcept(std::is_arithmetic_v<U>)
		{
			return other == static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
		USE_RESULT friend constexpr bool operator!=(const T& obj, const U& other) noexcept(std::is_arithmetic_v<U>)
		{
			return other != static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
			USE_RESULT friend constexpr bool operator!=(const U& other, const T& obj) noexcept(std::is_arithmetic_v<U>)
		{
			return other != static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
		USE_RESULT friend constexpr bool operator<(const T& obj, const U& other) noexcept(std::is_arithmetic_v<U>)
		{
			return other < static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
			USE_RESULT friend constexpr bool operator<(const U& other, const T& obj) noexcept(std::is_arithmetic_v<U>)
		{
			return other < static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
		USE_RESULT friend constexpr bool operator<=(const T& obj, const U& other) noexcept(std::is_arithmetic_v<U>)
		{
			return other <= static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
			USE_RESULT friend constexpr bool operator<=(const U& other, const T& obj) noexcept(std::is_arithmetic_v<U>)
		{
			return other <= static_cast<U>(obj);
		}
		

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
		USE_RESULT friend constexpr bool operator>(const T& obj, const U& other) noexcept(std::is_arithmetic_v<U>)
		{
			return other > static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
			USE_RESULT friend constexpr bool operator>(const U& other, const T& obj) noexcept(std::is_arithmetic_v<U>)
		{
			return other > static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
		USE_RESULT friend constexpr bool operator>=(const T& obj, const U& other) noexcept(std::is_arithmetic_v<U>)
		{
			return other >= static_cast<U>(obj);
		}

		template<typename U, class = std::enable_if_t<
			!std::is_same_v<T, U>
			>>
			USE_RESULT friend constexpr bool operator>=(const U& other, const T& obj) noexcept(std::is_arithmetic_v<U>)
		{
			return other >= static_cast<U>(obj);
		}
	};
}
