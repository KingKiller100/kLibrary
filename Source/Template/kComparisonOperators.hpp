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
					return std::forward<T1>(lhs) == static_cast<T2&&>(rhs);
				}
			};
			
			struct InequalityOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& lhs, T2&& rhs) const noexcept(noexcept(static_cast<T1&&>(lhs) != static_cast<T2&&>(rhs)))
				{
					return std::forward<T1>(lhs) != static_cast<T2&&>(rhs);
				}
			};
			
			struct GreaterThanOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& big, T2&& small) const noexcept(noexcept(static_cast<T1&&>(big) > static_cast<T2&&>(small)))
				{
					return std::forward<T1>(big) > static_cast<T2&&>(small);
				}
			};
			
			struct GreaterThanEqualOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& big, T2&& small) const noexcept(noexcept(static_cast<T1&&>(big) >= static_cast<T2&&>(small)))
				{
					return std::forward<T1>(big) >= static_cast<T2&&>(small);
				}
			};
			
			struct LessThanOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& small, T2&& big) const noexcept(noexcept(static_cast<T1&&>(small) < static_cast<T2&&>(big)))
				{
					return std::forward<T1>(small) < static_cast<T2&&>(big);
				}
			};
			
			struct LessThanEqualOperator
			{
				template<typename T1, typename T2>
				USE_RESULT constexpr bool operator()(T1&& small, T2&& big) const noexcept(noexcept(static_cast<T1&&>(small) <= static_cast<T2&&>(big)))
				{
					return std::forward<T1>(small) <= static_cast<T2&&>(big);
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