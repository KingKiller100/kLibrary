#pragma once
#include <chrono>

namespace klib
{
	namespace kDebug
	{
		template<class CharT>
		void IsDebuggerAttached(const CharT* filename, std::chrono::milliseconds = std::chrono::milliseconds(500)) noexcept;
		void BreakPoint() noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

