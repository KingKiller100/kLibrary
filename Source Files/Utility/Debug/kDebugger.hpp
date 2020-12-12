#pragma once

namespace klib
{
	namespace kDebug
	{
		void IsDebuggerAttached(const char* filename) noexcept;
		void BreakPoint() noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

