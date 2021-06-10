#pragma once

#include "kPathTraits.hpp"

namespace klib
{
	namespace kFileSystem
	{
		// Dynamic string buffer
		template<typename CharT, typename AllocT = std::allocator<CharT>>
		using DOSPathString = std::basic_string<CharT, DOSPathTraits<CharT>, AllocT>;

		template<typename CharT, typename AllocT = std::allocator<CharT>>
		using UnixPathString = std::basic_string<CharT, UnixPathTraits<CharT>, AllocT>;

		template<typename CharT, typename AllocT = std::allocator<CharT>>
		using PathString = std::basic_string<CharT, PathTraits<CharT>, AllocT>;
		
		// Visual string buffer
		template<typename CharT>
		using DOSPathStringView = std::basic_string_view<CharT, DOSPathTraits<CharT>>;

		template<typename CharT>
		using UnixPathStringView = std::basic_string_view<CharT, UnixPathTraits<CharT>>;

		template<typename CharT>
		using PathStringView = std::basic_string_view<CharT, PathTraits<CharT>>;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kFileSystem;
#endif
}
