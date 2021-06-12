#pragma once
#include <string>
#include <Windows.h>
#include "../../String/kStringConverter.hpp"

namespace klib::kDebug::compiler::visual_studio
{
	void BreakPointImpl() noexcept;
	bool IsDebuggerAttachedImpl() noexcept;

	template<typename CharT, typename Traits = std::char_traits<CharT>>
	void WriteToOutputWindowImpl(std::basic_string_view<CharT, Traits> msg)
	{
		if constexpr (std::is_same_v<CharT, char>)
		{
			::OutputDebugStringA(msg.data());
		}
		else if constexpr (std::is_same_v<CharT, wchar_t>)
		{
			::OutputDebugStringW(msg.data());
		}
		else
		{
			WriteToOutputWindowImpl(kString::Convert<wchar_t>(msg.data()));
		}
	}
}
