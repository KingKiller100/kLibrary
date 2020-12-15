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
			std::basic_string<Char_t> filename;
			filename.push_back(Char_t('f'));
			filename.push_back(Char_t('e'));

			if (Contains(specifier, filename))
			{
				return ToString(Convert<Char_t>( "{0}" ), path.filename().string<Char_t>());
			}
			else if (Contains(specifier, Char_t('f')))
			{
				return ToString(Convert<Char_t>("{0}"), path.stem().string<Char_t>());
			}
			else if (Contains(specifier, Char_t('e')))
			{
				return ToString(Convert<Char_t>("{0}"), path.extension().string<Char_t>());
			}
			else if (Contains(specifier, Char_t('d'))
				|| Contains(specifier, Char_t('p')))
			{
				return ToString(Convert<Char_t>("{0}"), path.parent_path().string<Char_t>());
			}

			return ToString(Convert<Char_t>("{0}"), path.string<Char_t>());
		}
	};
}
