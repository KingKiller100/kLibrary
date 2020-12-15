#pragma once

#include "kMutableSourceInfo.hpp"
#include "../../String/kToString.hpp"
#include "../../FileSystem/kFileSystemToString.hpp"

namespace klib::kString::stringify
{
	template<typename Char_t>
	class Identity<Char_t, kDebug::MutSourceInfo>
	{
	public:
		using Type = kDebug::MutSourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "MutSourceInfo:char does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::MutSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
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
				return ToString("{0}", sourceInfo.timeStamp);
			}

			return ToString("File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, kDebug::wMutSourceInfo>
	{
	public:
		using Type = kDebug::wMutSourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "MutSourceInfo:wchar_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::wMutSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(L"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
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
				return ToString(L"{0}", sourceInfo.timeStamp);
			}

			return ToString(L"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, kDebug::u16MutSourceInfo>
	{
	public:
		using Type = kDebug::u16MutSourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "MutSourceInfo:char16_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::u16MutSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(u"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
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
				return ToString(u"{0}", sourceInfo.timeStamp);
			}

			return ToString(u"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

	template<typename Char_t>
	class Identity<Char_t, kDebug::u32MutSourceInfo>
	{
	public:
		using Type = kDebug::u32MutSourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "MutSourceInfo:char32_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::u32MutSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(U"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
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
				return ToString(U"{0}", sourceInfo.timeStamp);
			}

			return ToString(U"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

#if __cpp_char8_t
	template<typename Char_t>
	class Identity<Char_t, kDebug::u8MutSourceInfo>
	{
	public:
		using Type = kDebug::u8MutSourceInfo;
		static_assert(std::is_same_v<Char_t, typename Type::Char_t>
			, "MutSourceInfo:char8_t does not match the string's char type");

		USE_RESULT static decltype(auto) MakeStr(const kDebug::u8MutSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString(u8"File: {0} Line: {1} Function: {2} Time Stamp: {3}"
					, sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
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
				return ToString(u8"{0}", sourceInfo.timeStamp);
			}

			return ToString(u8"File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};
#endif
}
