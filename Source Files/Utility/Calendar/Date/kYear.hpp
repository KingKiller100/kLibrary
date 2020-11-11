#pragma once

#include "kDay.hpp"
#include "kMonth.hpp"

#include "../Secret/kComponentToStringImpl.hpp"
#include "../../../HelperMacros.hpp"

#include <cstdint>


namespace klib::kCalendar
{	
	class Year final : private CalendarComponentToStringImplExtended
	{
	public:
		static constexpr auto FormatToken = 'y';
		static constexpr size_t LeapYearFrequency = 4;
		
		static constexpr size_t YearsInDecade = 10;
		static constexpr size_t YearsInCentury = 100;
		static constexpr size_t YearsInMillennium = 1000;
		
	public:
		constexpr explicit Year(const std::uint16_t year)
			: year(year)
		, isLeapYear((year % 4 == 0 // Year divisible by 4
		&& year % 100 != 0) // but not divisible by 100
		|| year % 400 == 0) // Unless it's divisible by 400
		{}

		~Year() noexcept = default;

		USE_RESULT constexpr std::uint16_t GetValue() const
		{
			return year;
		}

		USE_RESULT constexpr bool IsLeapYear() const
		{
			return isLeapYear;
		}

		USE_RESULT constexpr std::uint16_t TotalDays() const
		{
			return isLeapYear
				? Day::DaysInLeapYear
				: Day::DaysInYear;
		}
		
		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return GetValue();
		}
		
		USE_RESULT std::string GetYearStr() const;
		USE_RESULT std::string ToString(const std::string_view& format) const;

		friend class Date;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount( const size_t count ) const override;
		
	private:
		const std::uint16_t year;
		const bool isLeapYear;
	};

	constexpr Year operator"" _y(unsigned long long year)
	{
		return Year(static_cast<std::uint16_t>(year));
	}
}
