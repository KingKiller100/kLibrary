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

	template<>
	void IsDebuggerAttached(const char* filename, std::chrono::milliseconds delayTime) noexcept
	{
#if defined(KLIB_DEBUG) || defined(KLIB_TEST)
		if (IsDebuggerAttachedImpl())
			return;

		while (kFileSystem::CheckFileExists(filename))
		{
			if (IsDebuggerAttachedImpl() == KLIB_TRUE)
			{
				return;
			}
			std::this_thread::sleep_for(delayTime);
		}
#endif
	}
	template<>
	void IsDebuggerAttached(const wchar_t* filename, std::chrono::milliseconds delayTime) noexcept
	{
#if defined(KLIB_DEBUG) || defined(KLIB_TEST)
		if (IsDebuggerAttachedImpl())
			return;

		while (kFileSystem::CheckFileExists(filename))
		{
			if (IsDebuggerAttachedImpl() == KLIB_TRUE)
			{
				return;
			}
			std::this_thread::sleep_for(delayTime);
		}
#endif
	}

	void BreakPoint() noexcept
	{
		BreakPointImpl();
	}

}
