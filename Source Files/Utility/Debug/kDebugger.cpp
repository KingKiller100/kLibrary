#include "pch.hpp"
#include "kDebugger.hpp"

#include "Compiler/VisualStudio.hpp"
#include "../FileSystem/kFileSystem.hpp"
#include "../String/kStringConverter.hpp"

#include <thread>

namespace klib::kDebug
{
#ifdef MSVC_PLATFORM_TOOLSET
	using namespace compiler::vs;
#endif

	template<class CharT>
	void IsDebuggerAttachedTemplate(const CharT* filename, std::chrono::milliseconds delayTime) noexcept
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
	void IsDebuggerAttached(const char* filename, std::chrono::milliseconds delayTime) noexcept
	{
		return IsDebuggerAttachedTemplate(filename, delayTime);
	}
	
	template<>
	void IsDebuggerAttached(const wchar_t* filename, std::chrono::milliseconds delayTime) noexcept
	{
		return IsDebuggerAttachedTemplate(filename, delayTime);
	}
	
	template<>
	void IsDebuggerAttached(const char16_t* filename, std::chrono::milliseconds delayTime) noexcept
	{

#if __cpp_char8_t
		using CharType = char8_t;
#else
		using CharType = wchar_t;
#endif
		
		const std::basic_string<CharType> fname = kString::Convert<CharType>(filename);
		
		return IsDebuggerAttachedTemplate(fname.data(), delayTime);
	}
	
	template<>
	void IsDebuggerAttached(const char32_t* filename, std::chrono::milliseconds delayTime) noexcept
	{
		
#if __cpp_char8_t
		using CharType = char8_t;
#else
		using CharType = wchar_t;
#endif

		const std::basic_string<CharType> fname = kString::Convert<CharType>(filename);
		return IsDebuggerAttachedTemplate(fname.data(), delayTime);
	}

	
#if __cpp_char8_t
	template<>
	void IsDebuggerAttached(const char8_t* filename, std::chrono::milliseconds delayTime) noexcept
	{
		return IsDebuggerAttachedTemplate(filename, delayTime);
	}
#endif

	void BreakPoint() noexcept
	{
		BreakPointImpl();
	}

}
