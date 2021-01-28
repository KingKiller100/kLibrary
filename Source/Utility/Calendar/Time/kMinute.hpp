#pragma once

#include "kTimeComponentBase.hpp"
#include "../Secret/kComponentToStringImpl.hpp"

#include "../../../HelperMacros.hpp"
#include "../../../Template/kSimpleOperators.hpp"

#include <chrono>
#include <string>

namespace klib::kCalendar
{
	class Minute final : public TimeComponentBase<std::chrono::duration<std::int16_t, std::ratio<60>>>
		, private CalendarComponentToStringImplExtended
		, public kTemplate::SimpleComparisonOperators<Minute>
	{
	public:
		static constexpr std::string_view Units = "m";
		static constexpr char FormatToken = 'm';

	public:
		constexpr explicit Minute(const Rep_t& minute = 0)
			: TimeComponentBase( minute )
		{}

		USE_RESULT constexpr bool Verify() const
		{
			return VerifyImpl(60);
		}

		USE_RESULT constexpr void Limit()
		{
			LimitImpl(60);
		}
		
		USE_RESULT std::string ToString(const std::string_view& format = "m") const;

		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
	};

	constexpr Minute operator ""_mm(unsigned long long mins)
	{
		return Minute(static_cast<Minute::Rep_t>(mins));
	}
}

