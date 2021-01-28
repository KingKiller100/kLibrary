#pragma once

#include "kTimeComponentBase.hpp"
#include "../Secret/kComponentToStringImpl.hpp"

#include "../../../Template/kSimpleOperators.hpp"

#include <chrono>
#include <string>

namespace klib::kCalendar
{
	class Millisecond final : public TimeComponentBase<std::chrono::duration<std::int16_t, std::milli>>
		, private CalendarComponentToStringImplExtended
		, public kTemplate::SimpleComparisonOperators<Millisecond>
	{
	public:
		static constexpr std::string_view Units = "ms";
		static constexpr auto FormatToken = 'c';
		
	public:
		constexpr Millisecond(const Rep_t& mills = 0)
			: TimeComponentBase(mills)
		{}

		USE_RESULT std::string ToString(const std::string_view& format = "c") const;

		USE_RESULT constexpr bool Verify() const
		{
			return VerifyImpl(Duration_t::period::den);
		}

		USE_RESULT constexpr void Limit()
		{
			LimitImpl(Duration_t::period::den);
		}
		
		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
	};

	constexpr Millisecond operator"" _ms(unsigned long long millisecond)
	{
		return Millisecond(static_cast<Millisecond::Rep_t>(millisecond));
	}
}
