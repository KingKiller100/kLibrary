#pragma once
#include "TemplateTraits.hpp"
#include <string>

#include "../HelperMacros.hpp"

namespace klib::type_trait
{
	template<typename T>
	struct CharacterTraits : private std::char_traits<T>
	{
		using Base_t = std::char_traits<T>;

		// Character Compare Results
		enum CompareResult : short {
			FIRST_BAD = -1,
			EQUAL = 0,
			SECOND_BAD,
			DIFFERENT = 65535
		};
		
		USE_RESULT static constexpr size_t Length(const T* str) noexcept
		{
			return Base_t::length(str);
		}

		USE_RESULT static constexpr T* Copy(T* dst, const T* const src, const size_t size) noexcept
		{
			return Base_t::copy(dst, src, size);
		}

		USE_RESULT static constexpr  T* CopySafe(T* const dst,
			const size_t dstSize, const T* const src,
			const size_t srcSize) noexcept
		{
			if (srcSize > dstSize)
				std::_Xout_of_range("Src size is bigger than the dstSize");
			
			return Copy(dst, src, srcSize);
		}

		USE_RESULT static constexpr T* Move(T* const dst,
			const T* const src, const size_t _Count) noexcept
		{
			return Base_t::move(dst, src, _Count);
		}

		USE_RESULT static constexpr CompareResult CompareRaw(const T* left, size_t leftSize,
			const T* right, size_t rightSize) noexcept
		{
			const auto result = Base_t::compare(left, right, std::min(leftSize, rightSize));
			return static_cast<CompareResult>(result);
		}

		USE_RESULT static constexpr bool Compare(const T* left, size_t leftSize,
			const T* right, size_t rightSize) noexcept
		{
			const auto result = Base_t::compare(left, right, std::min(leftSize, rightSize));
			return result == 0;
		}
	};
	
	template<typename T>
	struct Is_CharTypeBase : std::bool_constant<Is_It_V<T,
		char, unsigned char, signed char, wchar_t, char16_t, char32_t
#ifdef __cpp_char8_t
		, char8_t
#endif
		>>
	{
		using Traits = CharacterTraits<T>;
		using Char_t = T;
	};

	template<typename T>
	struct Is_CharType : Is_CharTypeBase<std::remove_cv_t<T>>
	{};

	// Determines whether type is a character type
	template<typename T>
	constexpr bool Is_CharType_V = Is_CharType<T>::value;

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_Char_t = Is_CharType_V<T> == true;
#endif
	

	template<typename CharType, typename = std::enable_if_t<Is_CharType_V<CharType>>>
	constexpr auto g_NullTerminator = CharType();

}