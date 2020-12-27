#pragma once


#include "../Secret/kComponentToStringImpl.hpp"
#include "../../../Template/kSimpleOperators.hpp"

#include <array>
#include <string>

namespace klib::kCalendar
{
	class Year;
	class Day;

	class Month final : private CalendarComponentToStringImplExtended, public kTemplate::SimpleOperators<Month>
	{
	public:
		enum MonthOfTheYear : unsigned char
		{
			JAN = 1, FEB, MAR,
			APR, MAY, JUN,
			JUL, AUG, SEP,
			OCT, NOV, DEC,
		};

		static constexpr auto FormatToken = 'm';
		static constexpr size_t MonthsInYear = 12;

	public:
		constexpr explicit Month(MonthOfTheYear month = JAN)
			: moty(month)
		{}

		~Month() = default;

		USE_RESULT constexpr MonthOfTheYear GetValue() const
		{
			return moty;
		}

		USE_RESULT constexpr void SetValue(MonthOfTheYear m)
		{
			moty = m;
		}

		USE_RESULT constexpr std::uint16_t GetMonthNumber() const
		{
			return static_cast<std::uint16_t>(moty);
		}

		USE_RESULT static constexpr const char* MonthToString(MonthOfTheYear month)
		{
			constexpr std::array<const char*, MonthsInYear> kCalendar_MonthsArray = {
				"January", "February", "March",
				"April", "May", "June",
				"July", "August", "September",
				"October", "November", "December"
			};

			return kCalendar_MonthsArray[static_cast<size_t>(month) - 1];
		}

		template<typename Target_t, class = std::enable_if_t<
			std::is_arithmetic_v<Target_t>
			>>
		constexpr operator Target_t() const
		{
			return static_cast<Target_t>(GetMonthNumber());
		}

		USE_RESULT std::string ToString(const std::string_view& format) const;
		USE_RESULT bool Verify(const Day& day, const Year& year) const;

		friend class Date;

	protected:
		USE_RESULT std::string GetMonthStr() const;
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;

	private:
		MonthOfTheYear moty;
	};

	constexpr Month operator"" _m(unsigned long long month)
	{
		return Month(static_cast<Month::MonthOfTheYear>(month));
	}
}
