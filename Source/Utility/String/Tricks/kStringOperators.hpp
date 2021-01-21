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
			std::basic_string<Char_t, _Traits, _Alloc> str(writer);

			str.append(reader);

			return str;
		}

		template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const std::basic_string_view<Char_t, _Traits>& reader
			, const std::basic_string<Char_t, _Traits, _Alloc>& writer)
		{
			std::basic_string<Char_t, _Traits, _Alloc> str(reader);

			str.append(writer);

			return str;
		}

		template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const std::basic_string_view<Char_t, _Traits>& r1
			, const std::basic_string_view<Char_t, _Traits>& r2)
		{
			std::basic_string<Char_t, _Traits, _Alloc> str(r1);

			str.append(r2);

			return str;
		}

		// template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		// USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
		// 	const std::basic_string_view<Char_t, _Traits>& reader, const Char_t* cstr)
		// {
		// 	std::basic_string<Char_t, _Traits, _Alloc> str(reader);
		// 	str.append(cstr);
		// 	return str;
		// }
		//
		// template <class Char_t, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		// USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
		// 	const Char_t* cstr,
		// 	const std::basic_string_view<Char_t, _Traits>& reader)
		// {
		// 	std::basic_string<Char_t, _Traits, _Alloc> str(cstr);
		// 	str.append(reader);
		// 	return str;
		// }

		template <class Char_t, size_t Size, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const std::basic_string_view<Char_t, _Traits>& reader, const Char_t(&cstr)[Size])
		{
			std::basic_string<Char_t, _Traits, _Alloc> str(reader);
			str.append(cstr);
			return str;
		}

		template <class Char_t, size_t Size, class _Traits = std::char_traits<Char_t>, class _Alloc = std::allocator<Char_t>>
		USE_RESULT constexpr std::basic_string<Char_t, _Traits, _Alloc> operator+(
			const Char_t(&cstr)[Size],
			const std::basic_string_view<Char_t, _Traits>& reader)
		{
			std::basic_string<Char_t, _Traits, _Alloc> str(cstr);
			str.append(reader);
			return str;
		}

	}
}

#ifdef KLIB_SHORT_NAMESPACE
using namespace klib::kString::operators;
#endif
