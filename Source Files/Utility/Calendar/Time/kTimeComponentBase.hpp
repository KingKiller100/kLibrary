#pragma once
#include "../../../HelperMacros.hpp"

namespace klib::kCalendar
{
	template<typename DurationT>
	class TimeComponentBase
	{
	public:
		using Duration_t = DurationT;
		using Period_t = typename Duration_t::period;
		using Rep_t = typename Duration_t::rep;
		
	public:
		constexpr TimeComponentBase(const Rep_t& val)
			: value(val)
		{}

		USE_RESULT constexpr const Duration_t& GetDuration() const
		{
			return value;
		}

		USE_RESULT constexpr Duration_t& GetDuration()
		{
			return value;
		}
		
		USE_RESULT constexpr Rep_t GetValue() const
		{
			return value.count();
		}

		template<typename TargetType>
		constexpr operator TargetType() const
		{
			return static_cast<TargetType>(GetValue());
		}

	protected:
		USE_RESULT constexpr bool VerifyImpl(const Rep_t& limit) const
		{
			return GetValue() < limit;
		}
		
		constexpr void LimitImpl(const Rep_t& limit)
		{
			value %= limit;
		}

	protected:
		Duration_t value;
	};
}
