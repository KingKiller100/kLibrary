#pragma once

#include "../Secret/kComponentToStringImpl.hpp"

#include "kTimeComponentBase.hpp"

#include <chrono>
#include <string>

namespace klib::kCalendar
{
	class Hour final : public TimeComponentBase<std::chrono::hours>
		, private CalendarComponentToStringImplExtended
	{
	public:
		enum CycleType : std::uint8_t
		{
			CYCLE_12 = 12,
			CYCLE_24 = 24,
		};
		
		static constexpr std::string_view Units = "h";
		static constexpr auto FormatToken = 'h';

	public:
		constexpr explicit Hour(const RepT hour, const CycleType cycleType = CYCLE_24)
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

		USE_RESULT constexpr void Limit()
		{
			LimitImpl(cycleType);
		}

		USE_RESULT std::string ToString(const std::string_view& format = "h") const;

		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount(const size_t count) const override;
		
	private:
		CycleType cycleType;
	};

	constexpr Hour operator ""_hh(unsigned long long hours)
	{
		return Hour(static_cast<Hour::RepT>(hours), Hour::CYCLE_24);
	}
}