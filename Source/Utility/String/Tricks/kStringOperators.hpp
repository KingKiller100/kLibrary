#pragma once
#include "../../../HelperMacros.hpp"

#include <string>

namespace klib {
	namespace kString::operators
	{
		template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const std::basic_string<Char_t, _Traits, _Alloc>& writer
			, const std::basic_string_view<Char_t, _Traits>& reader)
		{
			return writer + reader.data();
		}

		template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const std::basic_string_view<Char_t, _Traits>& reader
			, const std::basic_string<Char_t, _Traits, _Alloc>& writer)
		{
			return reader.data() + writer;
		}

		template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const std::basic_string_view<Char_t, _Traits>& r1
			, const std::basic_string_view<Char_t, _Traits>& r2)
		{
			return r1.data() + std::basic_string<Char_t, _Traits, _Alloc>(r2);
		}
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kString;
#endif
}
