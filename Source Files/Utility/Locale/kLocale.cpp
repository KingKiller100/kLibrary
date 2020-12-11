#include "pch.hpp"
#include "kLocale.hpp"

namespace klib::kLocale
{
	std::locale g_kLocaleLocale;
	std::string g_kLocaleCurrentLocale("C");
	
	std::locale& GetLocale()
	{
		return g_kLocaleLocale;
	}

	const std::string& GetLocaleName()
	{
		return g_kLocaleCurrentLocale;
	}

	void SetLocale( const std::string_view& localeName, std::locale::category cat )
	{
		if ( g_kLocaleCurrentLocale != localeName )
		{
			g_kLocaleCurrentLocale = localeName;
			g_kLocaleLocale = std::locale( g_kLocaleCurrentLocale.data(), cat );
		}
	}
}
