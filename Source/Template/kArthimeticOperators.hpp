#pragma once

#include "../HelperMacros.hpp"

#include <xstddef>

namespace klib
{
	namespace kTemplate
	{
		struct ArithmeticOperators 
		{
		public:
			template<typename T1, typename T2>
			USE_RESULT static constexpr auto Add(T1&& lhs, T2&& rhs) noexcept(AdditionOperator(std::forward<T1>(lhs), std::forward<T2>(rhs)))
			{
				return AdditionOperator(std::forward<T1>(lhs), std::forward<T2>(rhs));
			}
			
			template<typename T1, typename T2>
			USE_RESULT static constexpr auto Subtract(T1&& lhs, T2&& rhs) noexcept(SubtractionOperator(std::forward<T1>(lhs), std::forward<T2>(rhs)))
			{
				return SubtractionOperator(std::forward<T1>(lhs), std::forward<T2>(rhs));
			}
			
			template<typename T1, typename T2>
			USE_RESULT static constexpr auto Multiply(T1&& lhs, T2&& rhs) noexcept(MultiplyOperator(std::forward<T1>(lhs), std::forward<T2>(rhs)))
			{
				return MultiplyOperator(std::forward<T1>(lhs), std::forward<T2>(rhs));
			}
			
			template<typename T1, typename T2>
			USE_RESULT static constexpr auto Divide(T1&& lhs, T2&& rhs) noexcept(std::forward<T1>(lhs) / std::forward<T2>(rhs))
			{
				return std::forward<T1>(lhs) / std::forward<T2>(rhs);
			}
			
			template<typename T>
			USE_RESULT static constexpr auto Increment(T&& value)
			{
				value = Add(std::forward<T>(value), T(1));
				return value;
			}
			
			template<typename T>
			USE_RESULT static constexpr auto Decrement(T&& value)
			{
				value = Subtract(std::forward<T>(value), T(1));
				return value;
			}
			
			template<typename T>
			USE_RESULT static constexpr auto Inverse(T&& value)
			{
				return static_cast<T>(T(1) / value);
			}
			
		private:
			static constexpr std::plus<void> AdditionOperator{};
			static constexpr std::minus<void> SubtractionOperator{};
			static constexpr std::multiplies<void> MultiplyOperator{};
		};

		
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kTemplate;
#endif

}
