#include "pch.hpp"
#include "Compiler.hpp"

namespace klib::kDebug::compiler
{
	iCompiler* GetCompiler()
	{
#ifdef MSVC_PLATFORM_TOOLSET
		return new VSCompiler();
#endif

	}
}
