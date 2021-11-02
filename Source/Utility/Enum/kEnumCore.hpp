#pragma once

#include "../../HelperMacros.hpp"

#include <array>
#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <stdexcept>

#define MAP(macro, ...) \
    IDENTITY( \
        APPLY(CHOOSE_MAP_START, COUNT(__VA_ARGS__)) \
            (macro, __VA_ARGS__))

#define CHOOSE_MAP_START(count) MAP ## count

#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))

#define IDENTITY(x) x

#define MAP1(m, x)      m(x)
#define MAP2(m, x, ...) m(x) IDENTITY(MAP1(m, __VA_ARGS__))
#define MAP3(m, x, ...) m(x) IDENTITY(MAP2(m, __VA_ARGS__))
#define MAP4(m, x, ...) m(x) IDENTITY(MAP3(m, __VA_ARGS__))
#define MAP5(m, x, ...) m(x) IDENTITY(MAP4(m, __VA_ARGS__))
#define MAP6(m, x, ...) m(x) IDENTITY(MAP5(m, __VA_ARGS__))
#define MAP7(m, x, ...) m(x) IDENTITY(MAP6(m, __VA_ARGS__))
#define MAP8(m, x, ...) m(x) IDENTITY(MAP7(m, __VA_ARGS__))
#define MAP9(m, x, ...) m(x) IDENTITY(MAP8(m, __VA_ARGS__))
#define MAP10(m, x, ...) m(x) IDENTITY(MAP9(m, __VA_ARGS__))
#define MAP11(m, x, ...) m(x) IDENTITY(MAP10(m, __VA_ARGS__))
#define MAP12(m, x, ...) m(x) IDENTITY(MAP11(m, __VA_ARGS__))
#define MAP13(m, x, ...) m(x) IDENTITY(MAP12(m, __VA_ARGS__))
#define MAP14(m, x, ...) m(x) IDENTITY(MAP13(m, __VA_ARGS__))
#define MAP15(m, x, ...) m(x) IDENTITY(MAP14(m, __VA_ARGS__))
#define MAP16(m, x, ...) m(x) IDENTITY(MAP15(m, __VA_ARGS__))
#define MAP17(m, x, ...) m(x) IDENTITY(MAP16(m, __VA_ARGS__))
#define MAP18(m, x, ...) m(x) IDENTITY(MAP17(m, __VA_ARGS__))
#define MAP19(m, x, ...) m(x) IDENTITY(MAP18(m, __VA_ARGS__))
#define MAP20(m, x, ...) m(x) IDENTITY(MAP19(m, __VA_ARGS__))
#define MAP21(m, x, ...) m(x) IDENTITY(MAP20(m, __VA_ARGS__))
#define MAP22(m, x, ...) m(x) IDENTITY(MAP21(m, __VA_ARGS__))
#define MAP23(m, x, ...) m(x) IDENTITY(MAP22(m, __VA_ARGS__))
#define MAP24(m, x, ...) m(x) IDENTITY(MAP23(m, __VA_ARGS__))
#define MAP25(m, x, ...) m(x) IDENTITY(MAP24(m, __VA_ARGS__))
#define MAP26(m, x, ...) m(x) IDENTITY(MAP25(m, __VA_ARGS__))
#define MAP27(m, x, ...) m(x) IDENTITY(MAP26(m, __VA_ARGS__))
#define MAP28(m, x, ...) m(x) IDENTITY(MAP27(m, __VA_ARGS__))
#define MAP29(m, x, ...) m(x) IDENTITY(MAP28(m, __VA_ARGS__))
#define MAP30(m, x, ...) m(x) IDENTITY(MAP29(m, __VA_ARGS__))
#define MAP31(m, x, ...) m(x) IDENTITY(MAP30(m, __VA_ARGS__))
#define MAP32(m, x, ...) m(x) IDENTITY(MAP31(m, __VA_ARGS__))
#define MAP33(m, x, ...) m(x) IDENTITY(MAP32(m, __VA_ARGS__))
#define MAP34(m, x, ...) m(x) IDENTITY(MAP33(m, __VA_ARGS__))
#define MAP35(m, x, ...) m(x) IDENTITY(MAP34(m, __VA_ARGS__))
#define MAP36(m, x, ...) m(x) IDENTITY(MAP35(m, __VA_ARGS__))
#define MAP37(m, x, ...) m(x) IDENTITY(MAP36(m, __VA_ARGS__))
#define MAP38(m, x, ...) m(x) IDENTITY(MAP37(m, __VA_ARGS__))
#define MAP39(m, x, ...) m(x) IDENTITY(MAP38(m, __VA_ARGS__))
#define MAP40(m, x, ...) m(x) IDENTITY(MAP39(m, __VA_ARGS__))

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, count, ...) \
    count

#define COUNT(...) \
    IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

// Stringizes each argument.
#define STRINGIZE_SINGLE(e) #e,
#define STRINGIZE(...) IDENTITY(MAP(STRINGIZE_SINGLE, __VA_ARGS__))

namespace klib::kEnum::secret::impl
{
	// The type "U" mentioned above that drops assignment operations.
	template <typename U>
	struct ignore_assign {
		constexpr explicit ignore_assign(U&& value) : _value(std::forward<U>(value)) { }
		constexpr operator U() const { return _value; }

		constexpr const ignore_assign& operator =(int) const
		{
			return *this;
		}

		U _value;
	};

	// Prepends "(ignore_assign<Underlying_t>)" to each argument.
#define IGNORE_ASSIGN_SINGLE(e) (klib::kEnum::secret::impl::ignore_assign<Underlying_t>)e,
#define IGNORE_ASSIGN(...) \
    IDENTITY(MAP(IGNORE_ASSIGN_SINGLE, __VA_ARGS__))

	// Some helpers needed for _from_string.
	constexpr char    terminators[] = " =\t\r\n";

	// The size of terminators includes the implicit '\0'.
	constexpr bool is_terminator(char c, size_t index = 0)
	{
		return
			index >= sizeof(terminators) ? false :
			c == terminators[index] ? true :
			is_terminator(c, index + 1);
	}

	constexpr bool matches_untrimmed(const char* untrimmed, const char* s,
		size_t index = 0)
	{
		return
			is_terminator(untrimmed[index]) ? s[index] == '\0' :
			s[index] != untrimmed[index] ? false :
			matches_untrimmed(untrimmed, s, index + 1);
	}
}