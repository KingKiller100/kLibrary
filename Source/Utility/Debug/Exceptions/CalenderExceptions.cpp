﻿#include "pch.hpp"
#include "CalenderExceptions.hpp"

namespace klib::kDebug
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Calendar Exception  Base //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CalendarError::CalendarError( std::string_view message )
		: ExceptionBase( message )
	{}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Invalid Day Error /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	InvalidDayError::InvalidDayError( std::string_view message )
		: CalendarError( message )
	{}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Invalid Day Error /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	InvalidMonthError::InvalidMonthError( std::string_view message )
		: CalendarError( message )
	{}
}
