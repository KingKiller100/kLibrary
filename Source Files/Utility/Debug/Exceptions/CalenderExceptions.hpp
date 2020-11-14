#pragma once

#include <stdexcept>

namespace klib::kDebug
{
	class CalendarError : public std::exception
	{
	public:
		explicit CalendarError();
		explicit CalendarError(const std::string& _Message);
		explicit CalendarError(const char* const _Message);

		~CalendarError() noexcept override = default;
	};

	class InvalidDayError final : public CalendarError
	{
	public:
		explicit InvalidDayError();
		explicit InvalidDayError(const std::string& _Message);
		explicit InvalidDayError(const char* const _Message);

		~InvalidDayError() noexcept override = default;
	};

	class InvalidMonthError final : public CalendarError
	{
	public:
		explicit InvalidMonthError();
		explicit InvalidMonthError(const std::string& _Message);
		explicit InvalidMonthError(const char* const _Message);

		~InvalidMonthError() noexcept override = default;
	};

	
}
