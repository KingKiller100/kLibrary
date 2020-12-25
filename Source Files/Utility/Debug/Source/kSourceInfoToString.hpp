#pragma once

#include "kSourceInfo.hpp"
#include "../../String/kToString.hpp"

namespace klib::kString::stringify
{
	template<typename Char_t>
	class Identity<Char_t, kDebug::SourceInfo>
	{
	public:
		using Type = kDebug::SourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "SourceInfo:char does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.stamp);
			}
			else if (Contains(specifier, "fl"))
			{
				return ToString("{0} [{1}]", sourceInfo.file, sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString("{0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString("{0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString("{0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString("{0}", sourceInfo.stamp);
			}

			return ToString("File: {0} Line: {1} Function: {2}"
				, sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, kDebug::wSourceInfo>
	{
	public:
		using Type = kDebug::wSourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "SourceInfo:wchar_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::wSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(L"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.stamp);
			}
			else if (Contains(specifier, L"fl"))
			{
				return ToString(L"{0} [{1}]", sourceInfo.file, sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString(L"{0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString(L"{0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString(L"{0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString(L"Time Stamp: {0}", sourceInfo.stamp);
			}

			return ToString(L"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, kDebug::u16SourceInfo>
	{
	public:
		using Type = kDebug::u16SourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "SourceInfo:char16_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::u16SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(u"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.stamp);
			}
			else if (Contains(specifier, u"fl"))
			{
				return ToString(u"{0} [{1}]", sourceInfo.file, sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString(u"{0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString(u"{0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString(u"{0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString(u"Time Stamp: {0}", sourceInfo.stamp);
			}

			return ToString(u"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, kDebug::u32SourceInfo>
	{
	public:
		using Type = kDebug::u32SourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "SourceInfo:char32_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::u32SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(U"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.stamp);
			}
			else if (Contains(specifier, U"fl"))
			{
				return ToString(U"{0} [{1}]", sourceInfo.file, sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString(U"{0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString(U"{0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString(U"{0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString(U"Time Stamp: {0}", sourceInfo.stamp);
			}

			return ToString(U"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

#if __cpp_char8_t
	template<typename Char_t>
	class Identity<Char_t, kDebug::u8SourceInfo>
	{
	public:
		using Type = kDebug::u8SourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "SourceInfo:char8_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::u8SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(u8"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.stamp);
			}
			else if (Contains(specifier, u8"fl"))
			{
				return ToString(u8"{0} [{1}]", sourceInfo.file, sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString(u8"{0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString(u8"{0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString(u8"{0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString(u8"Time Stamp: {0}", sourceInfo.stamp);
			}

			return ToString(u8"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};
#endif
}
