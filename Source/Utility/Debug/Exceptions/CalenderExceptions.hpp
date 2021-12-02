#pragma once

#include "ExceptionBase.hpp"
#include <stdexcept>

namespace klib::kDebug
{
	class CalendarError : public ExceptionBase
	{
	public:
		explicit CalendarError(std::string_view message = "Calendar Error");

		~CalendarError() noexcept override = default;
	};

	class InvalidDayError final : public CalendarError
	{
	public:
		explicit InvalidDayError(std::string_view message = "Invalid Day Of The Week Error");

		~InvalidDayError() noexcept override = default;
	};

	class InvalidMonthError final : public CalendarError
	{
	public:
		explicit InvalidMonthError(std::string_view message = "Invalid Month Of The Year Error");

		~InvalidMonthError() noexcept override = default;
	};

	
}
