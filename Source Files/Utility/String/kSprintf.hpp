#pragma once

#include "kStringConverter.hpp"
#include "Format/kStringIdentity.hpp"
#include "../../HelperMacros.hpp"

#include <cstdio>
#include <string>
#include <stdexcept>

#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4996)

namespace klib::kString
{
	template<typename CharType, typename T, typename ...Ts>
	USE_RESULT constexpr std::basic_string<CharType> SprintfWrapper(const CharType* format, const T& arg1, const Ts& ...argPack)
	{
		using SignedSize_t = std::make_signed_t<size_t>;
		
		constexpr SignedSize_t npos(0);

		CharType* buffer = nullptr;
		auto length = npos;

		if _CONSTEXPR_IF(std::is_same_v<CharType, char>)
		{
			length = _snprintf(nullptr, 0, format
				, stringify::Identity<CharType, T>::Get(arg1)
				, stringify::Identity<CharType, Ts>::Get(argPack)...) + 1;
			if (length <= npos) throw std::runtime_error("Error during char type \"" __FUNCSIG__ "\" formatting: string returned length <= 0");
			buffer = new CharType[length]();
			sprintf_s(buffer, length, format
				, stringify::Identity<CharType, T>::Get(arg1)
				, stringify::Identity<CharType, Ts>::Get(argPack)...);
		}
		else if _CONSTEXPR_IF(std::is_same_v<CharType, wchar_t>)
		{
			length = _snwprintf(nullptr, 0, format
				, stringify::Identity<CharType, T>::Get(arg1)
				, stringify::Identity<CharType, Ts>::Get(argPack)...) + 1;
			if (length <= npos) throw std::runtime_error("Error during wchar_t type \"" __FUNCSIG__ "\" formatting: string returned length <= 0");
			buffer = new CharType[length]();
			swprintf_s(buffer, length, format
				, stringify::Identity<CharType, T>::Get(arg1)
				, stringify::Identity<CharType, Ts>::Get(argPack)...);
		}
		else
		{
			const auto fmt = kString::Convert<wchar_t>(format);
			const auto str = SprintfWrapper<wchar_t>(fmt, arg1, argPack...);
			const auto text = kString::Convert<CharType>(str);
			return text;
		}

		const auto formattedText = std::basic_string<CharType>(buffer, buffer + length - 1);
		delete[] buffer;
		
		return formattedText;
	}

	template<typename CharType, typename T, typename ...Ts>
	USE_RESULT constexpr std::basic_string<CharType> SprintfWrapper(const std::basic_string_view<CharType>& format, const T& arg, const Ts& ...argPack)
	{
		return SprintfWrapper<CharType>(format.data(), arg, argPack...);
	}

	template<typename CharType, typename T, typename ...Ts>
	USE_RESULT constexpr std::basic_string<CharType> SprintfWrapper(const std::basic_string<CharType>& format, const T& arg, const Ts& ...argPack)
	{
		return SprintfWrapper<CharType>(format.data(), arg, argPack...);
	}
	
}

#	pragma warning(pop)
#endif