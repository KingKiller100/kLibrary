#include "pch.hpp"
#include "VisualStudio.hpp"


namespace klib::kDebug::compiler::visual_studio
{
	void BreakPointImpl() noexcept
	{
		__debugbreak();
	}

	bool IsDebuggerAttachedImpl() noexcept
	{
		return ::IsDebuggerPresent() == TRUE;
	}
}
