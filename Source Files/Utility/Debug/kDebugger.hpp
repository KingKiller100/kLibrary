#pragma once

namespace klib
{
	namespace kDebug
	{
		void CheckRemoteDebuggerAttached(const char* filename) noexcept;

		void BreakPointHere() noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

