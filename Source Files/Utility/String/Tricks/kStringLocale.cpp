#include "pch.hpp"
#include "kStringLocale.hpp"


namespace klib::kString::secret::impl
{
	std::locale s_Locale;
	std::string s_CurrentLocale("C");
	
	std::locale& GetLocale()
	{
		return s_Locale;
	}

	const std::string& GetLocaleName()
	{
		return s_CurrentLocale;
	}

	void SetLocale( const std::string_view& localeName, std::locale::category cat )
	{
		if ( s_CurrentLocale != localeName )
		{
			s_CurrentLocale = localeName;
			s_Locale = std::locale( s_CurrentLocale.data(), cat );
		}
	}
}
