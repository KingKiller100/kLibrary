#pragma once

#include "../Secret/kComponentToStringImpl.hpp"
#include "kTimeComponentBase.hpp"

#include <chrono>
#include <string>

namespace klib::kCalendar
{
	class Second final : public TimeComponentBase<std::chrono::seconds>
	, private CalendarComponentToStringImplExtended
	{
	public:
		static constexpr std::string_view Units = "s";
		static constexpr auto FormatToken = 's';
		
	public:
		constexpr Second(const RepT second)
			: TimeComponentBase( second )
		{}

		USE_RESULT constexpr bool Verify() const
		{
			return VerifyImpl(60);
		}

		USE_RESULT constexpr void Limit()
		{
			LimitImpl(60);
		}
		
		USE_RESULT std::string ToString(const std::string_view& format = "s") const;
		
		friend class Time;

	protected:
		USE_RESULT std::string ToStringUsingTokenCount( const size_t count ) const override;
	};

	constexpr Second operator"" _ss(unsigned long long second)
	{
		return Second(static_cast<Second::RepT>(second));
	}
}
