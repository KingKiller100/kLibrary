#include "pch.hpp"
#include "kDebugger.hpp"

#include <chrono>
#include <chrono>

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

	bool WaitForDebugger( const kFileSystem::Path& flagName, std::chrono::milliseconds refreshTime ) noexcept
	{
		if ( IsDebuggerAttached() )
			return true;

		bool attached = false;
		while ( kFileSystem::CheckFileExists( flagName ) )
		{
			attached = IsDebuggerAttachedImpl();
			if ( attached )
			{
				break;
			}
			std::this_thread::sleep_for( refreshTime );
		}

		return attached;
	}

	bool ScanForDebugger( std::chrono::milliseconds waitTime )
	{
		using namespace std::chrono;

		if ( IsDebuggerAttachedImpl() )
			return true;

		const auto endTimePoint = high_resolution_clock::now() + waitTime;

		while ( endTimePoint >= high_resolution_clock::now() )
		{
			if ( IsDebuggerAttachedImpl() )
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

	void WriteToOutputWindow( std::string_view msg )
	{
		WriteToOutputWindowImpl( msg );
	}

	void WriteToOutputWindow( std::wstring_view msg )
	{
		WriteToOutputWindowImpl( msg );
	}
}
