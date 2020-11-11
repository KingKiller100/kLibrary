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
#else
#	define UNUSED 
#	define USE_RESULT 
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

#define ONLY_TYPE(type) std::decay_t<std::remove_pointer_t<type>>

#define BIT_SHIFT(n) (1 << n)

// Class member getter & setters
#define GETTER(type, name, field) USE_RESULT type& name() noexcept\
{\
	return field;\
}\

#define CONST_GETTER(type, name, field) USE_RESULT const type& name() const noexcept\
{\
	return field;\
}\

#define SETTER(type, name, field) void name(const type& value) noexcept\
{\
	(field) = value;\
}\


// Class member getter & setters constexpr
#define GETTER_CONSTEXPR(type, name, field) USE_RESULT constexpr type& name() noexcept\
{\
	return field;\
}\

#define CONST_GETTER_CONSTEXPR(type, name, field) USE_RESULT constexpr const type& name() const noexcept\
{\
	return field;\
}\

#define SETTER_CONSTEXPR(type, name, field) constexpr void name(const type& value) noexcept\
{\
	(field) = value;\
}\

#if defined(KLIB_TEST)
#	define TESTING_ENABLED 1
#endif // KLIB_TEST
