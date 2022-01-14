#pragma once

#include "kDay.hpp"

#include "../Secret/kComponentToStringImpl.hpp"
#include "../../../Template/kSimpleOperators.hpp"
#include "../../../HelperMacros.hpp"

#include <cstdint>

namespace klib::kCalendar
{
	constexpr bool CheckLeapYear( const size_t year ) noexcept
	{
		return ( ( year % 4 == 0     // Year divisible by 4
				&& year % 100 != 0 ) // but not divisible by 100
			|| year % 400 == 0 );    // Unless it's divisible by 400
	}

	class Year final : private CalendarComponentToStringImpl, public kTemplate::SimpleComparisonOperators<Year>
	{
	public:
		static constexpr auto FormatToken = 'y';
		static constexpr size_t LeapYearFrequency = 4;

		static constexpr size_t YearsInDecade = 10;
		static constexpr size_t YearsInCentury = 100;
		static constexpr size_t YearsInMillennium = 1000;

	public:
		constexpr explicit Year( const std::uint16_t year = 1970 )
			: year( year )
		{}

		~Year() noexcept = default;

		USE_RESULT constexpr std::uint16_t GetValue() const
		{
			return year;
		}

		USE_RESULT constexpr void SetValue( std::uint16_t y )
		{
			year = y;
		}

		USE_RESULT constexpr bool IsLeapYear() const
		{
			return CheckLeapYear( static_cast<size_t>( year ) );
		}

		USE_RESULT constexpr std::uint16_t TotalDays() const
		{
			return IsLeapYear()
				       ? Day::DaysInLeapYear
				       : Day::DaysInYear;
		}

		template <typename Target_t, class = std::enable_if_t<
			          std::is_arithmetic_v<Target_t>
		          >>
		constexpr operator Target_t() const
		{
			return static_cast<Target_t>( GetValue() );
		}

		USE_RESULT std::string GetYearStr() const;
		USE_RESULT std::string ToString( const std::string_view& format ) const;

		friend class Date;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount( size_t count ) const;

	private:
		std::uint16_t year;
	};
}
