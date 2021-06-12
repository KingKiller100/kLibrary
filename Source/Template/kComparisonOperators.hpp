#pragma once
#include "../HelperMacros.hpp"

#include <type_traits>

namespace klib
{
	namespace kTemplate
	{
		namespace operators
		{
			struct EqualityOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs) == static_cast<T2&&>(rhs)))
				{
					return static_cast<T1&&>(lhs) == static_cast<T2&&>(rhs);
				}
			};
			
			struct InequalityOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs) != static_cast<T2&&>(rhs)))
				{
					return static_cast<T1&&>(lhs) != static_cast<T2&&>(rhs);
				}
			};
			
			struct GreaterThanOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& bigger, T2&& smaller) const noexcept(noexcept(static_cast<T1&&>(bigger) > static_cast<T2&&>(smaller)))
				{
					return static_cast<T1&&>(bigger) > static_cast<T2&&>(smaller);
				}
			};
			
			struct GreaterThanEqualOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& bigger, T2&& smaller) const noexcept(noexcept(static_cast<T1&&>(bigger) >= static_cast<T2&&>(smaller)))
				{
					return static_cast<T1&&>(bigger) >= static_cast<T2&&>(smaller);
				}
			};
			
			struct LessThanOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& smaller, T2&& bigger) const noexcept(noexcept(static_cast<T1&&>(smaller) < static_cast<T2&&>(bigger)))
				{
					return static_cast<T1&&>(smaller) < static_cast<T2&&>(bigger);
				}
			};
			
			struct LessThanEqualOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& smaller, T2&& bigger) const noexcept(noexcept(static_cast<T1&&>(smaller) <= static_cast<T2&&>(bigger)))
				{
					return static_cast<T1&&>(smaller) <= static_cast<T2&&>(bigger);
				}
			};
		}

		struct ComparisonOperators
		{
			static constexpr operators::EqualityOperator Equality{};
			static constexpr operators::InequalityOperator Inequality{};
			static constexpr operators::LessThanOperator Less{};
			static constexpr operators::LessThanEqualOperator LessEqual{};
			static constexpr operators::GreaterThanOperator Greater{};
			static constexpr operators::GreaterThanEqualOperator GreaterEqual{};
		};
	}
}