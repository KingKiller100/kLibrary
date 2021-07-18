#include "pch.hpp"
#include "kDebugger.hpp"

#include "Compiler/VisualStudio.hpp"
#include "../FileSystem/kFileSystem.hpp"

#include <thread>

namespace klib::kDebug
{
#ifdef MSVC_PLATFORM_TOOLSET
	using namespace compiler::visual_studio;
#endif

	bool IsDebuggerAttached() noexcept
	{
		return IsDebuggerAttachedImpl();
	}

	void WaitForDebugger(const kFileSystem::Path& flagName, std::chrono::milliseconds refreshTime) noexcept
	{
		if (IsDebuggerAttached())
			return;
		
		while (kFileSystem::CheckFileExists(flagName))
		{
			if (IsDebuggerAttachedImpl())
			{
				return;
			}
			std::this_thread::sleep_for(refreshTime);
		}
	}

	bool ScanForDebugger(std::chrono::milliseconds waitTime)
	{
		using namespace std::chrono;
		
		if (IsDebuggerAttachedImpl())
			return true;

		const auto endTimePoint = high_resolution_clock::now() + waitTime;
		
		while (endTimePoint >= high_resolution_clock::now())
		{
			if (IsDebuggerAttachedImpl())
			{
				return true;
			}
		}
		
		return false;
	}

	void BreakPoint() noexcept
	{
		BreakPointImpl();
	}

	void WriteToOutputWindow(std::string_view msg)
	{
		WriteToOutputWindowImpl(msg);
	}

	void WriteToOutputWindow(std::wstring_view msg)
	{
		WriteToOutputWindowImpl(msg);
	}

}
