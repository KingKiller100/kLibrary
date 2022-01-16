#pragma once
#include "../FileSystem/kFileSystemTypes.hpp"
#include <chrono>

namespace klib
{
	namespace kDebug
	{
		enum class RuntimeConfiguration
		{
			DEBUG, RELEASE
		};

		constexpr RuntimeConfiguration GetRuntimeConfiguration() noexcept
		{
			return
#if !defined(NDEBUG) || defined(KLIB_DEBUG)
				RuntimeConfiguration::DEBUG;
#else
				RuntimeConfiguration::RELEASE;
#endif
		}

		constexpr auto GetRuntimeConfigurationStr() noexcept
		{
			return GetRuntimeConfiguration() == RuntimeConfiguration::DEBUG
				? "Debug"
				: "Release";
		}

		bool IsDebuggerAttached() noexcept;

		bool WaitForDebugger( const kFileSystem::Path& path, std::chrono::milliseconds refreshTime = std::chrono::milliseconds( 500 ) ) noexcept;

		bool ScanForDebugger(std::chrono::milliseconds waitTime);

		void BreakPoint() noexcept;

		void WriteToOutputWindow(std::basic_string_view<char> msg);
		void WriteToOutputWindow(std::basic_string_view<wchar_t> msg);
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

