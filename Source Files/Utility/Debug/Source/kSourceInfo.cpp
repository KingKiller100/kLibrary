#include "pch.hpp"
#include "kSourceInfo.hpp"

#include "../../String/kToString.hpp"

namespace klib::kDebug
{
	template<>
	USE_RESULT std::basic_string<char> BasicSourceInfo<char>::ToString() const
	{
		return kString::ToString("File: {0} Line: {2} Function: {1}", file, func, line);
	}

	template<>
	USE_RESULT std::basic_string<wchar_t> BasicSourceInfo<wchar_t>::ToString() const
	{
		return kString::ToString(L"File: {0} Line: {2} Function: {1}", file, func, line);
	}

	template<>
	USE_RESULT std::basic_string<char16_t> BasicSourceInfo<char16_t>::ToString() const
	{
		return kString::ToString(u"File: {0} Line: {2} Function: {1}", file, func, line);
	}
	template<>
	USE_RESULT std::basic_string<char32_t> BasicSourceInfo<char32_t>::ToString() const
	{
		return kString::ToString(U"File: {0} Line: {2} Function: {1}", file, func, line);
	}

#ifdef __cpp_char8_t
	template<>
	USE_RESULT std::basic_string<char8_t> BasicSourceInfo<char8_t>::ToString() const
	{
		return kString::ToString(u8"File: {0} Line: {2} Function: {1}", file, func, line);
	}
#endif

}