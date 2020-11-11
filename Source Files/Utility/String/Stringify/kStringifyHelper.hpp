#pragma once

namespace klib::kString::stringify
{
	constexpr auto nPrecision = static_cast<size_t>(-1);

	template<typename CharType>
	void PrependPadding(std::basic_string<CharType>& outStr, const size_t minDigits, CharType padding)
	{
		if (outStr.size() >= minDigits)
			return;

		const auto count = minDigits - outStr.size();
		outStr.insert(outStr.begin(), count, padding);
	}
}
