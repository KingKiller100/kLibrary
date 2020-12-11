#include "pch.hpp"
#include "VisualStudio.hpp"
#include "../../../HelperMacros.hpp"

#include <Windows.h>

namespace klib::kDebug::compiler::vs
{
	bool IsDebuggerPresent() noexcept
	{
		return ::IsDebuggerPresent() == KLIB_TRUE;
	}

	void BreakPoint() noexcept
	{
		__debugbreak();
	}
}
