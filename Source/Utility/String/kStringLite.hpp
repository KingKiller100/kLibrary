#pragma once

#include "kStringTricks.hpp"

namespace klib::kString
{
	template<class CharT, bool Editable>
	struct StringLite
	{
		using String_t = std::basic_string<CharT>;
		static constexpr auto nPos = String_t::npos;
		
		StringLite(const String_t& s)
			: str(s)
		{}

		template<typename T>
		constexpr operator T() const
		{
			return str;
		}

		USE_RESULT constexpr String_t& Str() const
		{
			return str;
		}

		USE_RESULT constexpr String_t& Str()
		{
			return str;
		}
		
	public:
		String_t str;
	};

	template<class CharT>
	struct StringLite<std::basic_string_view<CharT>, false>
	{
		using String_t = std::basic_string_view<CharT>;
		static constexpr auto nPos = String_t::npos;

		StringLite(const String_t& s)
			: str(s)
		{}

		template<typename T>
		constexpr operator T() const
		{
			return str;
		}

		USE_RESULT constexpr const String_t& Str() const
		{
			return str;
		}

		USE_RESULT constexpr String_t& Str()
		{
			return str;
		}
		
	public:
		String_t str;
	};
}
