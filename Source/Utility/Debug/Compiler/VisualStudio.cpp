#include "pch.hpp"
#include "VisualStudio.hpp"
#include "../../../HelperMacros.hpp"

#include <Windows.h>

namespace klib::kDebug::compiler::vs
{
	void BreakPointImpl() noexcept
	{
		__debugbreak();
	}

	bool IsDebuggerAttachedImpl() noexcept
	{
		return ::IsDebuggerPresent() == KLIB_TRUE;
	}
}
