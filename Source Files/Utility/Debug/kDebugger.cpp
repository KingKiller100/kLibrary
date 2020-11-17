#include "pch.hpp"
#include "kDebugger.hpp"

#include "../FileSystem/kFileSystem.hpp"

namespace klib::kDebug
{
	void CheckRemoteDebuggerAttached(const char* filename) noexcept
	{
#ifdef  _DEBUG
		if (::IsDebuggerPresent() == KLIB_TRUE)
			return;

		while (kFileSystem::CheckFileExists(filename))
		{
			if (::IsDebuggerPresent() == KLIB_TRUE)
			{
				return;
			}
		}
#endif
	}

	void BreakPointHere() noexcept
	{
		__debugbreak();
	}

}