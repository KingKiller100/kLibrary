#pragma once

#include "../../HelperMacros.hpp"

#include "../String/kStringConverter.hpp"

#include <array>
#include <memory>
#include <string>
#include <string_view>

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

#define EVALUATE_COUNT(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, count, ...) \
    count

#define COUNT(...) \
    IDENTITY(EVALUATE_COUNT(__VA_ARGS__, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

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

// Prepends "(ignore_assign<underlying_t>)" to each argument.
#define IGNORE_ASSIGN_SINGLE(e) (klib::kEnum::secret::impl::ignore_assign<underlying_t>)e,
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

#define ENUM_X(structure_t, enumName, underlying, ...)									\
structure_t enumName																	\
{																						\
	using underlying_t = underlying;													\
public:																					\
	enum enum_t : underlying_t { __VA_ARGS__ };											\
																						\
private:																				\
	struct secret_impl_##enumName														\
	{																					\
		static constexpr auto size = IDENTITY(COUNT(__VA_ARGS__));						\
																						\
		static constexpr std::array<underlying_t, size> values =						\
		{ IDENTITY(IGNORE_ASSIGN(__VA_ARGS__)) };										\
																						\
		static constexpr std::array<std::string_view, size> raw_names =					\
		{ IDENTITY(STRINGIZE(__VA_ARGS__)) };											\
	};																					\
																						\
public:																					\
	constexpr enumName(const enum_t value)												\
		: value(value)																	\
	{}																					\
																						\
	constexpr enumName& operator=(const enum_t value)									\
	{																					\
		this->value = value;															\
		return *this;																	\
	}																					\
																						\
	constexpr operator enum_t() const													\
	{																					\
		return static_cast<enum_t>(value);												\
	}																					\
																						\
	USE_RESULT constexpr enum_t ToEnum() const											\
	{																					\
		return static_cast<enum_t>(value);												\
	}																					\
																						\
	USE_RESULT constexpr underlying_t ToUnderlying() const								\
	{																					\
		return value;																	\
	}																					\
																						\
	USE_RESULT constexpr bool MaskCmp(enum_t target) const								\
	{																					\
		return MaskCmp(target, true, false);											\
	}																					\
																						\
	template<typename T1, typename T2, typename = std::enable_if_t<						\
	std::is_convertible_v<T2, T1>														\
	>>																					\
	USE_RESULT constexpr std::decay_t<T1> MaskCmp(enum_t mask							\
		, T1&& successState, T2&& failState) const										\
	{																					\
		if (mask & value)																\
			return successState;														\
		return failState;																\
	}																					\
																						\
	USE_RESULT constexpr bool Compare(enum_t target) const								\
	{																					\
		return Compare(target, true, false);											\
	}																					\
																						\
	template<typename T1, typename T2>													\
	USE_RESULT constexpr std::decay_t<T1> Compare(enum_t target							\
		, T1&& successState, T2&& failState) const										\
	{																					\
		if (target == value)															\
			return successState;														\
		return failState;																\
	}																					\
																						\
	static constexpr enum_t FromString(const std::string_view& search)					\
	{																					\
		return FromStringImpl(search.data(), 0);										\
	}																					\
																						\
	USE_RESULT																			\
	static std::string PrettyValue(enumName input)										\
	{																					\
		static const auto type = std::string(PrettyType()) + "::";						\
		const auto name = std::string(input.ToString<char>());							\
		const auto prettyValue = type + name;											\
		return prettyValue;																\
	}																					\
																						\
	USE_RESULT																			\
	static constexpr std::string_view PrettyType()										\
	{																					\
		return "enum " #enumName;														\
	}																					\
																						\
	template<class Char_t = char>														\
	USE_RESULT std::basic_string_view<Char_t> ToString() const							\
	{																					\
		const auto name = ToStringImpl<Char_t>();										\
		return name;																	\
	}																					\
																						\
	private:																			\
	template<class Char_t>																\
	USE_RESULT const Char_t* ToStringImpl() const										\
	{																					\
		const auto* name = TrimmedNames<Char_t>();										\
		return name;																	\
	}																					\
																						\
	template<class Char_t>																\
	USE_RESULT const Char_t* TrimmedNames() const										\
	{																					\
		static std::unique_ptr<Char_t[]> the_names[secret_impl_##enumName::size];		\
		static bool  initialized = false;												\
		size_t index = 0;																\
																						\
		if (!initialized)																\
		{																				\
			InitializeNames<Char_t>(the_names);											\
			initialized = true;															\
		}																				\
																						\
		do																				\
		{																				\
			if (ToEnum() == secret_impl_##enumName::values[index])						\
				break;																	\
		}																				\
		while (++index < secret_impl_##enumName::size);									\
																						\
		return the_names[index].get();													\
	}																					\
																						\
	template<class Char_t>																\
	constexpr void InitializeNames(std::unique_ptr<Char_t[]>							\
		(&the_names)[secret_impl_##enumName::size]) const								\
	{																					\
		using namespace klib::kEnum::secret::impl;										\
		for (auto i = 0; i < secret_impl_##enumName::size; ++i)							\
		{																				\
			const auto & raw_name = secret_impl_##enumName::raw_names[i];				\
			auto& name = the_names[i];													\
																						\
			const auto length_til_terminator =											\
				std::strcspn(raw_name.data(),											\
					terminators);														\
																						\
			name.reset(new Char_t[length_til_terminator + 1]);							\
																						\
			size_t index(0);															\
			while (index < length_til_terminator)										\
			{																			\
				name[index] = Char_t(raw_name[index]);									\
				++index;																\
			}																			\
																						\
			name[length_til_terminator] = Char_t('\0');									\
		}																				\
	}																					\
																						\
	static constexpr enum_t FromStringImpl(const char* s, size_t index)					\
	{																					\
		using namespace klib::kEnum::secret::impl;										\
																						\
		if (index >= secret_impl_##enumName::size)										\
			std::_Xout_of_range("Name does not map to a value in enum: " #enumName);	\
																						\
		const auto matches =															\
			matches_untrimmed(															\
				secret_impl_##enumName::raw_names[index].data(), s);					\
																						\
		const auto ret = matches														\
			? static_cast<enum_t>(secret_impl_##enumName::values[index])				\
			: FromStringImpl(s, index + 1);												\
																						\
		return ret;																		\
	}																					\
																						\
private:																				\
	underlying_t value;																	\
};																						\

#define ENUM_CLASS(enumName, underlying, ...) ENUM_X(class, enumName, underlying, __VA_ARGS__)
#define ENUM_STRUCT(enumName, underlying, ...) ENUM_X(struct, enumName, underlying, __VA_ARGS__)
