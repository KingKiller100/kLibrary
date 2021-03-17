#pragma once
#include <chrono>

namespace klib
{
	namespace kDebug
	{
		constexpr bool IsRuntimeDebug() noexcept
		{
			return 
#if !defined(NDEBUG) || defined(KLIB_DEBUG)
			true;
#else

			false;
#endif
		}
		
		template<class CharT>
		void IsDebuggerAttached(const CharT* filename, std::chrono::milliseconds = std::chrono::milliseconds(500)) noexcept;
		void BreakPoint() noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

