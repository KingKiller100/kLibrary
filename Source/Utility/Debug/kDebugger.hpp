#pragma once
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
		
		template<class CharT>
		void IsDebuggerAttached(const CharT* filename, std::chrono::milliseconds = std::chrono::milliseconds(500)) noexcept;
		void BreakPoint() noexcept;
		
		void WriteToOutputWindow(std::basic_string_view<char> msg);
		void WriteToOutputWindow(std::basic_string_view<wchar_t> msg);
	}

#ifdef KLIB_SHORT_NAMESPACE
	using namespace kDebug;
#endif
}

