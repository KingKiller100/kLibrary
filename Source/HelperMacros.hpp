#pragma once

#if KLIB_LIB
//#	if KLIB_DYNAMIC_LINK
//#		ifdef KLIB_BUILD_DLL
//#			define KLIB_API __declspec(dllexport)
//#			define PORT_LIB
//#		else
//#			define KLIB_API __declspec(dllimport)
//#			define PORT_LIB extern
//#		endif
//#	else
//#		define KLIB_API 
//#		define PORT_LIB 
//#	endif
#endif

#if _MSVC_LANG > 201402L
#	define UNUSED [[maybe_unused]]
#	define USE_RESULT [[nodiscard]]
#	define NO_RETURN [[noreturn]]
#else
#	define UNUSED 
#	define USE_RESULT 
#	define NO_RETURN
#	if MSVC_PLATFORM_TOOLSET < 110
#		define noexcept throw ()
#		define constexpr 
#	endif
#endif

#define KLIB_TRUE 1
#define KLIB_FALSE 0

#define CAST(type, value) static_cast<type>(value)
#define REINTERPRET(type, value) reinterpret_cast<type>(value)
#define DYNAMIC(type, value) dynamic_cast<type>(value)

#define ONLY_TYPE(type) std::remove_cv_t<std::remove_pointer_t<std::decay_t<type>>>

#define BIT_SHIFT(n) (static_cast<size_t>(1) << n)

#define kSOURCEINFOIMPL_CONCATENATE(x,y) kSOURCEINFOIMPL_CONCATENATE2(x,y) 
#define kSOURCEINFOIMPL_CONCATENATE2(x,y) x##y 
#define kFILELINE size_t(kSOURCEINFOIMPL_CONCATENATE(__LINE__, U))

#ifndef TESTING_ENABLED
#	define TESTING_ENABLED KLIB_TRUE
#endif

