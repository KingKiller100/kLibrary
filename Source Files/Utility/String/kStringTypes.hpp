#pragma once

#include "../../HelperMacros.hpp"

#include <string>

namespace klib::kString
{
	//Type aliases for STL containers --------------------------------------------------------

	// STL basic_string_view
	template<class Char>
	using StringReader = std::basic_string_view<ONLY_TYPE(Char)>;

	// STL basic_string
	template<class Char>
	using StringWriter = std::basic_string<ONLY_TYPE(Char), std::char_traits<ONLY_TYPE(Char)>, std::allocator<ONLY_TYPE(Char)>>;

	// --------------------------------------------------------------------------------------

	template<class Char>
	USE_RESULT constexpr StringWriter<Char> ToWriter(const Char* string) noexcept
	{
		return StringWriter<Char>(string);
	}

	template<class StringT, typename = std::enable_if_t<type_trait::Is_StringType_V<StringT>>>
	USE_RESULT constexpr StringWriter<typename StringT::value_type> ToWriter(const StringT& string) noexcept
	{
		return StringWriter<typename StringT::value_type>(string);
	}

	template<class Char>
	USE_RESULT constexpr StringReader<Char> ToReader(const Char* string) noexcept
	{
		return StringReader<Char>(string);
	}

	template<class StringT, typename = std::enable_if_t<type_trait::Is_StringType_V<StringT>>>
	USE_RESULT constexpr StringReader<typename StringT::value_type> ToReader(const StringT& string) noexcept
	{
		return StringReader<typename StringT::value_type>(string);
	}
	
}
