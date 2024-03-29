﻿#pragma once

#include "kTimeComponentBase.hpp"

#include "../Secret/kComponentToStringImpl.hpp"

#include "../../../Template/kSimpleOperators.hpp"

#include <chrono>
#include <string>


namespace klib::kCalendar
{
	class Hour final : public TimeComponentBase<std::chrono::duration<std::int16_t, std::ratio<3600>>>
		, private CalendarComponentToStringImpl
		, public kTemplate::SimpleComparisonOperators<Hour>
	{
	public:
		enum CycleType : std::uint8_t
		{
			CYCLE_12 = 12
			, CYCLE_24 = 24
			,
		};

		static constexpr std::string_view Units = "h";
		static constexpr auto FormatToken = 'h';

	public:
		constexpr explicit Hour(const Rep_t& hour = 0, const CycleType cycleType = CYCLE_24)
			: TimeComponentBase(hour)
			, cycleType(cycleType)
		{}

		USE_RESULT constexpr CycleType GetCycleType() const
		{
			return cycleType;
		}

		USE_RESULT constexpr bool Verify() const
		{
			return VerifyImpl(cycleType);
		}

		USE_RESULT constexpr void Normalize()
		{
			NormalizeImpl(cycleType);
		}

		USE_RESULT std::string ToString(const std::string_view& format = "h") const;

		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(size_t count) const;

	private:
		CycleType cycleType;
	};
}
