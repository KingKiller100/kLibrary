#pragma once

#include "kFileSystemTypes.hpp"
#include "../String/kToString.hpp"

namespace klib::kString::stringify
{
	template<typename Char_t>
	class Identity<Char_t, kFileSystem::Path>
	{
	public:
		using Type = kFileSystem::Path;

		USE_RESULT static decltype(auto) MakeStr(const Type& path, StringWriter<Char_t>& specifier)
		{
			StringReader<Char_t> format;
			StringReader<Char_t> filenameToken;

			if _CONSTEXPR17(std::is_same_v<Char_t, char>)
			{
				format = "{0}";
				filenameToken = "fe";
			}
			else if _CONSTEXPR17(std::is_same_v<Char_t, wchar_t>)
			{
				format = L"{0}";
				filenameToken = L"fe";
			}
			else if _CONSTEXPR17(std::is_same_v<Char_t, char16_t>)
			{
				format = u"{0}";
				filenameToken = u"fe";
			}
			else if _CONSTEXPR17(std::is_same_v<Char_t, char32_t>)
			{
				format = U"{0}";
				filenameToken = U"fe";
			}
#if __cpp_char8_t
			else if _CONSTEXPR17(std::is_same_v<Char_t, char32_t>)
			{
				format = u8"{0}";
				filenameToken = u8"fe";
			}
#endif

			if (Contains(specifier, filenameToken))
			{
				return ToString(format, path.filename().string<Char_t>());
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString(format, path.stem().string<Char_t>());
			}
			else if (Contains(specifier, Char_t('e')))
			{
				return ToString(format, path.extension().string<Char_t>());
			}
			else if (Contains(specifier, Char_t('d'))
				|| Contains(specifier, Char_t('p')))
			{
				return ToString(format, path.parent_path().string<Char_t>());
			}

			return ToString(format, path.string<Char_t>());
		}
	};
}
