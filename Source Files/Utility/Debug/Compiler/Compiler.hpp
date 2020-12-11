#pragma once


namespace klib::kDebug::compiler
{
	class iCompiler
	{
		virtual bool IsDebuggerPresent() noexcept = 0;
		virtual void BreakPoint() noexcept = 0;
	};

	iCompiler* GetCompiler();
}
