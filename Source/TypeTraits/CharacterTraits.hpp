#pragma once
#include "TemplateTraits.hpp"
#include "TraitsBase.hpp"
#include "../HelperMacros.hpp"
#include <string>

namespace klib::type_trait
{
	template<class T>
	struct Traits<T, std::enable_if_t<Is_It_V<T,
		char, wchar_t, char16_t, char32_t
#ifdef __cpp_char8_t
		, char8_t
#endif
		>>>
	{
		using Type = T;
		using Limits_t = std::numeric_limits<Type>;
		using Underlying_t = std::char_traits<T>;
		using Integer_t = typename Underlying_t::int_type;
		
		static constexpr auto NullTerminator = T();

		// Character Compare Results
		enum CompareResult : std::int8_t
		{
			FIRST_BAD = -1,
			EQUAL = 0,
			SECOND_BAD
		};
		
		USE_RESULT static constexpr size_t Length(const T* str) noexcept
		{
			return Underlying_t::length(str);
		}

		USE_RESULT static constexpr T* Copy(T* dst, const T* const src, const size_t size) noexcept
		{
			return Underlying_t::copy(dst, src, size);
		}

		USE_RESULT static constexpr  T* CopySafe(T* const dst,
			const size_t dstSize, const T* const src,
			const size_t srcSize) noexcept
		{
			if (srcSize > dstSize)
				std::_Xout_of_range("srcSize is bigger than the dstSize");
			
			return Copy(dst, src, srcSize);
		}

		USE_RESULT static constexpr T* Move(T* const dst,
			const T* const src, const size_t _Count) noexcept
		{
			return Underlying_t::move(dst, src, _Count);
		}

		USE_RESULT static constexpr CompareResult CompareRaw(const T* left, size_t leftSize,
			const T* right, size_t rightSize) noexcept
		{
			const auto result = Underlying_t::compare(left, right, (std::min)(leftSize, rightSize));
			return static_cast<CompareResult>(result);
		}

		USE_RESULT static constexpr bool Compare(const T* left, size_t leftSize,
			const T* right, size_t rightSize) noexcept
		{
			const auto result = Underlying_t::compare(left, right, (std::min)(leftSize, rightSize));
			return result == 0;
		}

		USE_RESULT static constexpr T* Assign(T* const str, size_t count, const T c) noexcept
		{
			return Underlying_t::assign(str, count, c);
		}

		static constexpr void Assign(T& left, const T& right) noexcept {
			Underlying_t::assign(left, right);
		}

		USE_RESULT static constexpr bool Eq(const T& left, const T& right) noexcept {
			return Underlying_t::eq(left, right);
		}

		USE_RESULT static constexpr bool Lt(const T& left, const T& right) noexcept {
			return Underlying_t::lt(left, right);
		}

		USE_RESULT static constexpr bool Gt(const T& left, const T& right) noexcept {
			return left > right;
		}

		USE_RESULT static constexpr T ToChar(const Integer_t& integer) noexcept {
			return Underlying_t::to_char_type(integer);
		}

		USE_RESULT static constexpr Integer_t ToInt(const T& c) noexcept {
			return Underlying_t::to_int_type(c);
		}

		USE_RESULT static constexpr bool Eq_Int(const Integer_t& left, const Integer_t& right) noexcept {
			return Underlying_t::eq_int_type(left, right);
		}

		USE_RESULT static constexpr Integer_t Not_Eof(const Integer_t& integer) noexcept {
			return Underlying_t::not_eof(integer);
		}

		USE_RESULT static constexpr Integer_t Eof() noexcept {
			return Underlying_t::eof();
		}
	};
	
	template<typename T>
	struct Is_CharacterBase : std::bool_constant<Is_It_V<T,
		char, unsigned char, signed char, wchar_t
#ifdef __cpp_char8_t
		, char8_t
#endif
	, char16_t, char32_t
		>>
	{
		using Traits = Traits<T>;
		using Type = T;
	};

	template<typename T>
	struct Is_Character : Is_CharacterBase<std::remove_cv_t<T>>
	{};

	// Determines whether type is a character type
	template<typename T>
	constexpr bool Is_Char_V = Is_Character<T>::value;

#if MSVC_PLATFORM_TOOLSET > 141
	template<typename T>
	concept Is_Char_c = Is_Char_V<T> == true;
#endif
	
	template<typename CharType, typename = std::enable_if_t<Is_Char_V<CharType>>>
	constexpr auto g_NullTerminator = Traits<CharType>::NullTerminator;

}