﻿#pragma once
#include "Length_Type.hpp"
#include "kMathsConstants.hpp"
#include "kMathsApproximately.hpp"

#include "../HelperMacros.hpp"


#ifdef max
#	undef max
#endif

#ifdef min
#	undef min
#endif

namespace kmaths
{
	namespace secret::impl
	{
		template <typename T>
		using ClosestFloat_t = std::conditional_t<
			std::is_floating_point_v<T>
			, T
			, std::conditional_t<
				sizeof( size_t ) == 4
				, float, double>>;

		template <typename T> // Unsigned
		USE_RESULT constexpr uint8_t Sign_Impl( const T x, std::false_type ) noexcept
		{
			return ( T( 0 ) < x );
		}

		template <typename T> // Signed
		USE_RESULT constexpr int8_t Sign_Impl( const T x, std::true_type ) noexcept
		{
			return ( T( 0 ) < x ) - ( x < T( 0 ) );
		}
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr decltype(auto) Sign( const T x ) noexcept
	{
		using namespace kmaths::secret::impl;
		return Sign_Impl( x, std::is_signed<T>() );
	}

	template <typename T>
	USE_RESULT constexpr bool IsNegative( T x ) noexcept
	{
		return x < 0;
	}

	template <typename T>
	USE_RESULT constexpr bool IsPositive( T x ) noexcept
	{
		return !IsNegative( x );
	}

	template <typename Dest_t, typename Source_t>
	USE_RESULT constexpr Dest_t Convert( Source_t&& source )
	{
		if constexpr ( std::is_arithmetic_v<Dest_t> )
		{
			constexpr auto max = std::numeric_limits<Dest_t>::max();
			constexpr auto min = std::numeric_limits<Dest_t>::min();

			if ( source > max )
				return max;
			else if ( source < min )
				return min;

			return Dest_t( source );
		}
		else
		{
			return Dest_t( std::forward<Source_t>( source ) );
		}
	}

	template <typename T>
	USE_RESULT constexpr T Fibonacci( const T n ) noexcept
	{
		if ( n <= 1 )
			return n;

		return ( Fibonacci( n - 1 ) + Fibonacci( n - 2 ) );
	}

	template <typename T1, typename T2, typename Pred>
	USE_RESULT constexpr std::common_type_t<T1, T2> Compare( const T1& lhs, const T2& rhs, Pred pred ) noexcept
	{
		using common_t = std::common_type_t<T1, T2>;
		return pred( static_cast<common_t>( lhs ), static_cast<common_t>( rhs ) )
			       ? static_cast<common_t>( lhs )
			       : static_cast<common_t>( rhs );
	}

	template <typename T1, typename T2>
	USE_RESULT constexpr auto Max( const T1& lhs, const T2& rhs ) noexcept
	{
		using common_t = std::common_type_t<T1, T2>;
		return Compare( lhs, rhs, std::greater<common_t>{} );
	}

	template <typename T1, typename T2>
	USE_RESULT constexpr auto Min( const T1& lhs, const T2& rhs ) noexcept
	{
		using common_t = std::common_type_t<T1, T2>;
		return Compare( lhs, rhs, std::less<common_t>{} );
	}

	template <typename T, size_t Size>
	USE_RESULT constexpr size_t SizeOfCArray( UNUSED const T (&)[Size] ) noexcept
	{
		return Size;
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr bool IsInteger( T value ) noexcept
	{
		if ( IsNegative( value ) )
			return CAST( BigInt_t, value ) == value;
		else
			return CAST( size_t, value ) == value;
	}

	template <typename T>
	USE_RESULT constexpr T Abs( T x ) noexcept
	{
		if constexpr ( std::is_unsigned_v<T> )
			return x;
		else
		{
			if ( x >= 0 )
				return x;

			if constexpr ( std::is_integral_v<T> )
				return ~x + constants::One<T>;
			else
				return -x;
		}
	}

	template <typename T, class = std::enable_if_t<std::is_arithmetic_v<T>>>
	USE_RESULT constexpr bool IsDecimal( T value ) noexcept
	{
		using namespace constants;
		constexpr auto one = One<T>;
		constexpr auto minusOne = MinusOne<T>;

		if ( ApproximatelyZero<T>( value ) )
			return false;

		return ( ( value > minusOne )
			&& ( value < one ) );
	}

	template <typename T>
	USE_RESULT constexpr T Remap( T progress, T actualMin, T actualMax, T remappedMin, T remappedMax ) noexcept
	{
		const T actualDifference = actualMax - actualMin;
		const T remappedDifference = remappedMax - remappedMin;
		const T actualProgress = ( progress - actualMin ) / actualDifference;
		const T remappedProgress = remappedMin + remappedMax * actualProgress;

		return remappedProgress;
	}

	template <typename T>
	USE_RESULT constexpr bool InRange( const T value, const T minVal, const T maxVal ) noexcept
	{
		return ( value >= minVal ) && ( value < maxVal );
	}

	template <typename T, class = std::enable_if_t<std::is_floating_point_v<T>>>
	USE_RESULT constexpr T Floor( const T value ) noexcept
	{
		if ( value < 0 )
		{
			constexpr auto maxVal = std::numeric_limits<BigInt_t>::max();
			constexpr auto minVal = std::numeric_limits<BigInt_t>::min();

			if ( value > maxVal || value < minVal )
				return value;

			const auto integer = CAST( T, CAST(BigInt_t, value) );
			return integer - constants::One<T>;
		}
		else
		{
			constexpr auto maxVal = std::numeric_limits<size_t>::max();
			if ( value > maxVal )
				return value;
			const auto integer = CAST( T, CAST(size_t, value) );
			return integer;
		}
	}

	template <typename T, class = std::enable_if_t<
		          !std::is_rvalue_reference_v<T>
		          && std::is_nothrow_move_assignable_v<T>
		          && std::is_nothrow_move_constructible_v<T>
	          >>
	constexpr void Swap( T& lhs, T& rhs ) noexcept
	{
		T temp = std::move( lhs );
		lhs = std::move( rhs );
		rhs = std::move( temp );
	}

	template <typename T>
	constexpr void Promote( T& x )
	{
		x *= 10;
	}

	template <typename T>
	constexpr void Demote( T& x )
	{
		x /= 10;
	}
}
