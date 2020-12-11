#include "pch.hpp"
#include "SourceInfo.hpp"

#include "../../String/kToString.hpp"

namespace klib::kDebug
{
	template<>
	USE_RESULT  std::basic_string<char> BasicSourceInfo<char>::ToString() const
	{
		return kString::SprintfWrapper("File: %s Function: %s Line: %d", file, func, line);
	}

	template<>
	USE_RESULT  std::basic_string<wchar_t> BasicSourceInfo<wchar_t>::ToString() const
	{
		return kString::SprintfWrapper(L"File: %s Function: %s Line: %d", file, func, line);
	}

	template<>
	USE_RESULT  std::basic_string<char16_t> BasicSourceInfo<char16_t>::ToString() const
	{
		return kString::SprintfWrapper(u"File: %s Function: %s Line: %d", kString::Convert<wchar_t>(file), kString::Convert<wchar_t>(func), line);
	}

	template<>
	USE_RESULT  std::basic_string<char32_t> BasicSourceInfo<char32_t>::ToString() const
	{
		return kString::SprintfWrapper(U"File: %s Function: %s Line: %d", kString::Convert<wchar_t>(file), kString::Convert<wchar_t>(func), line);
	}

#ifdef __cpp_char8_t
	template<>
	USE_RESULT  std::basic_string<char8_t> BasicSourceInfo<char8_t>::ToString() const
	{
		return kString::SprintfWrapper(u8"File: %s Function: %s Line: %d", kString::Convert<wchar_t>(file), kString::Convert<wchar_t>(func), line);
	}
#endif

}