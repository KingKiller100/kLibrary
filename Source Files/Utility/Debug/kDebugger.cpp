#include "pch.hpp"
#include "kDebugger.hpp"

#include "Compiler/VisualStudio.hpp"
#include "../FileSystem/kFileSystem.hpp"
#include <thread>


namespace klib::kDebug
{
	void CheckRemoteDebuggerAttached(const char* filename) noexcept
	{
#ifdef  _DEBUG
		if ()
			return;

		while (kFileSystem::CheckFileExists(filename))
		{
			if (::IsDebuggerPresent() == KLIB_TRUE)
			{
				return;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
#endif
	}

	void Break() noexcept
	{
#ifdef MSVC_PLATFORM_TOOLSET
		compiler::vs::BreakPoint();
#endif
	}

}
