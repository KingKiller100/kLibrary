#include "pch.hpp"
#include "kLocale.hpp"


namespace klib::kLocale
{
	namespace
	{
		std::locale g_Locale;
		std::string g_CurrentLocale("C");
	}
	std::locale& GetLocale()
	{
		return g_Locale;
	}

	const std::string& GetLocaleName()
	{
		return g_CurrentLocale;
	}

	void SetLocale(const std::string_view& localeName, std::locale::category cat)
	{
		if (g_CurrentLocale != localeName)
		{
			g_CurrentLocale = localeName;
			g_Locale = std::locale(g_CurrentLocale.data(), cat);
		}
	}
}
