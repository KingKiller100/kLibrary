#pragma once

#include "../Format/kStringIdentity.hpp"

#include <vector>

namespace klib::kString::stringify
{
	template<typename Char_t, typename T, typename Alloc>
	class Identity<Char_t, std::vector<T, Alloc>>
	{
	public:
		using Type = std::vector<T>;

		static constexpr auto ValueTypeTokenBreak = Char_t('~');

		USE_RESULT static decltype(auto) MakeStr(const Type& vector, StringWriter<Char_t>& specifier)
		{
			const auto tokenPos = specifier.find(ValueTypeTokenBreak);
			StringWriter<Char_t> vtSpecifier;

			if (tokenPos != type_trait::g_NoPos<StringWriter<Char_t>>)
			{
				vtSpecifier = specifier.substr(tokenPos + 1);
				specifier.erase(tokenPos);
			}

			StringWriter<Char_t> output;

			const auto size = vector.size();
			for (auto i = 0; i < size; ++i)
			{
				if (i > 0)
				{
					output += specifier;
					output.push_back(Char_t(' '));
				}

				output += Identity<Char_t, T>::MakeStr(vector[i], vtSpecifier);
			}

			return output;
		}
	};
}
