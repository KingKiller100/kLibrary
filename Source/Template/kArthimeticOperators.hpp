#pragma once

#include "../HelperMacros.hpp"

#include <type_traits>

namespace klib
{
	namespace kTemplate
	{
		namespace operators
		{
			struct AdditionOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr auto operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs) + static_cast<T2&&>(rhs)))
					-> decltype(static_cast<T1&&>(lhs) + static_cast<T2&&>(rhs))
				{
					return std::forward<T1>(lhs) + std::forward<T2>(rhs);
				}

				template<typename StdContainer>
				USE_RESULT typename StdContainer::value_type operator()(const StdContainer& container
					, typename StdContainer::value_type initialVal) const
				{
					auto result(initialVal);
					for (auto&& value : container)
					{
						result = operator()(result, value);
					}
					return result;
				}
			};
			struct SubtractionOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr auto operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs) - static_cast<T2&&>(rhs)))
					-> decltype(static_cast<T1&&>(lhs) - static_cast<T2&&>(rhs))
				{
					return std::forward<T1>(lhs) - std::forward<T2>(rhs);
				}

				template<typename StdContainer>
				USE_RESULT typename StdContainer::value_type operator()(const StdContainer& container
					, typename StdContainer::value_type initialVal) const
				{
					auto result(initialVal);
					for (auto&& value : container)
					{
						result = operator()(result, value);
					}
					return result;
				}
			};
			struct MultiplicationOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr auto operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs)* static_cast<T2&&>(rhs)))
					-> decltype(static_cast<T1&&>(lhs)* static_cast<T2&&>(rhs))
				{
					return std::forward<T1>(lhs) * std::forward<T2>(rhs);
				}

				template<typename StdContainer>
				USE_RESULT typename StdContainer::value_type operator()(const StdContainer& container
					, typename StdContainer::value_type initialVal) const
				{
					auto result(initialVal);
					for (auto&& value : container)
					{
						result = operator()(result, value);
					}
					return result;
				}
			};
			struct DivisionOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr auto operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs) / static_cast<T2&&>(rhs)))
					-> decltype(static_cast<T1&&>(lhs) / static_cast<T2&&>(rhs))
				{
					return std::forward<T1>(lhs) / std::forward<T2>(rhs);
				}

				template<typename StdContainer>
				USE_RESULT typename StdContainer::value_type operator()(const StdContainer& container
					, typename StdContainer::value_type initialVal) const
				{
					auto result(initialVal);
					for (auto&& value : container)
					{
						result = operator()(result, value);
					}
					return result;
				}
			};
			struct IncrementOperator
			{
				template<typename T>
				constexpr T operator()(T& value) const
				{
					return ++value;
				}
				
				template<typename T, typename U>
				constexpr T operator()(T& value, const U& amount) const
				{
					value = value + T(amount);
					return value;
				}
			};
			struct DecrementOperator
			{
				template<typename T>
				constexpr T operator()(T& value) const
				{
					return --value;
				}

				template<typename T, typename U>
				constexpr T operator()(T& value, const U& amount) const
				{
					value = value - T(amount);
					return value;
				}
			};
		}
		
		struct ArithmeticOperators 
		{
			static constexpr operators::AdditionOperator Addition{};
			static constexpr operators::SubtractionOperator Subtraction{};
			static constexpr operators::MultiplicationOperator Multiply{};
			static constexpr operators::IncrementOperator Increment{};
			static constexpr operators::DecrementOperator Decrement{};
		};
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kTemplate;
#endif

}
