#include "pch.hpp"
#include "kCalenderInfoSourceWindows.hpp"

#include "../../../Debug Helper/Exceptions/CalenderExceptions.hpp"

#ifdef _WIN64

namespace klib::kCalendar::secret::helper::windows
{
	CalendarInfoSourceWindows::CalendarInfoSourceWindows()
	{
		CalendarInfoSourceWindows::Refresh(CalendarInfoSourceType::LOCAL);
	}

	CalendarInfoSourceWindows::~CalendarInfoSourceWindows()
		= default;

	void CalendarInfoSourceWindows::Refresh(CalendarInfoSourceType type)
	{
		switch (type)
		{
		case CalendarInfoSourceType::SYSTEM:
			RefreshSystemInfo();
			break;
		case CalendarInfoSourceType::LOCAL:
			RefreshLocalInfo();
			break;
		default:
			throw kDebug::CalendarError("Unknown calendar source");
			break;
		}
	}

	void CalendarInfoSourceWindows::RefreshLocalInfo() noexcept
	{
		GetLocalTime(&dateTime);
	}

	void CalendarInfoSourceWindows::RefreshSystemInfo() noexcept
	{
		GetSystemTime(&dateTime);
	}

	std::uint16_t CalendarInfoSourceWindows::GetDay() const
	{
		return dateTime.wDay;
	}

	std::uint16_t CalendarInfoSourceWindows::GetDayOfTheWeekIndex() const
	{
		return dateTime.wDayOfWeek;
	}

	std::uint16_t CalendarInfoSourceWindows::GetMonth() const
	{
		return dateTime.wMonth;
	}

	std::uint16_t CalendarInfoSourceWindows::GetYear() const
	{
		return dateTime.wYear;
	}

	std::uint16_t CalendarInfoSourceWindows::GetHour() const
	{
		return dateTime.wHour;
	}

	std::uint16_t CalendarInfoSourceWindows::GetMinute() const
	{
		return dateTime.wMinute;
	}

	std::uint16_t CalendarInfoSourceWindows::GetSecond() const
	{
		return dateTime.wSecond;
	}

	std::uint16_t CalendarInfoSourceWindows::GetMillisecond() const
	{
		return dateTime.wMilliseconds;
	}

}
#endif