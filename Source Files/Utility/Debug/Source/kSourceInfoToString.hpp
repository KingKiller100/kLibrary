#pragma once

#include "kSourceInfo.hpp"
#include "../../String/kToString.hpp"

namespace klib::kString::stringify
{
	template<typename Char_t>
	class Identity<Char_t, kDebug::SourceInfo>
	{
	public:
		USE_RESULT static decltype(auto) MakeStr(const kDebug::SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} [{1}] Function: {2} Time Stamp: {3}"
				                , sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString("File: {0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString("Line: {0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString("Function: {0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString("Time Stamp: {0}", sourceInfo.timeStamp);
			}

			return ToString("File: {0} [{1}] Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};
	
	template<typename Char_t>
	class Identity<Char_t, kDebug::wSourceInfo>
	{
	public:
		USE_RESULT static decltype(auto) MakeStr(const kDebug::wSourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} Line: {1} Function: {2} Time Stamp: {3}"
				                , sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString("File: {0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString("Line: {0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString("Function: {0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString("Time Stamp: {0}", sourceInfo.timeStamp);
			}

			return ToString("File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};
	
	template<typename Char_t>
	class Identity<Char_t, kDebug::u16SourceInfo>
	{
	public:
		USE_RESULT static decltype(auto) MakeStr(const kDebug::u16SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} Line: {1} Function: {2} Time Stamp: {3}"
				                , sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString("File: {0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString("Line: {0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString("Function: {0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString("Time Stamp: {0}", sourceInfo.timeStamp);
			}

			return ToString("File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};
	
	template<typename Char_t>
	class Identity<Char_t, kDebug::u32SourceInfo>
	{
	public:
		USE_RESULT static decltype(auto) MakeStr(const kDebug::u32SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} Line: {1} Function: {2} Time Stamp: {3}"
				                , sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString("File: {0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString("Line: {0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString("Function: {0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString("Time Stamp: {0}", sourceInfo.timeStamp);
			}

			return ToString("File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};

#if __cpp_char8_t
	template<typename Char_t>
	class Identity<Char_t, kDebug::u8SourceInfo>
	{
	public:
		USE_RESULT static decltype(auto) MakeStr(const kDebug::u8SourceInfo& sourceInfo, StringWriter<Char_t>& specifier)
		{
			if (Contains(specifier, Char_t('a')))
			{
				return ToString("File: {0} Line: {1} Function: {2} Time Stamp: {3}"
				                , sourceInfo.file, sourceInfo.line, sourceInfo.func, sourceInfo.timeStamp);
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString("File: {0}", sourceInfo.file);
			}
			else if (Contains(specifier, Char_t('l')))
			{
				return ToString("Line: {0}", sourceInfo.line);
			}
			else if (Contains(specifier, Char_t('z')))
			{
				return ToString("Function: {0}", sourceInfo.func);
			}
			else if (Contains(specifier, Char_t('t')))
			{
				return ToString("Time Stamp: {0}", sourceInfo.timeStamp);
			}

			return ToString("File: {0} Line: {1} Function: {2}", sourceInfo.file, sourceInfo.line, sourceInfo.func);
		}
	};
#endif
}
