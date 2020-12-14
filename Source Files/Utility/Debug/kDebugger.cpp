#include "pch.hpp"
#include "kDebugger.hpp"

#include "Compiler/VisualStudio.hpp"
#include "../FileSystem/kFileSystem.hpp"
#include <thread>

namespace klib::kDebug
{
#ifdef MSVC_PLATFORM_TOOLSET
	using namespace compiler::vs;
#endif
	
	void IsDebuggerAttached(const char* filename) noexcept
	{
#if defined(_DEBUG) || defined(KLIB_DEBUG) || defined(KLIB_TEST)
		if (IsDebuggerAttachedImpl())
			return;

		while (kFileSystem::CheckFileExists(filename))
		{
			if (IsDebuggerAttachedImpl() == KLIB_TRUE)
			{
				return;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
#endif
	}

	void BreakPoint() noexcept
	{
		BreakPointImpl();
	}

}
