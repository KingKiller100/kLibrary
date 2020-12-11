#pragma once

namespace klib
{
	namespace kDebug
	{
		void CheckRemoteDebuggerAttached(const char* filename) noexcept;
		void Break() noexcept;
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

