#pragma once

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

namespace klib::kEnum::secret::helper
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
#define IGNORE_ASSIGN_SINGLE(e) (klib::kEnum::secret::helper::ignore_assign<underlying_t>)e,
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

	constexpr bool matches_untrimmed(const std::string_view& untrimmed, const std::string_view& s,
		size_t index = 0)
	{
		return
			is_terminator(untrimmed[index]) ? s[index] == '\0' :
			s[index] != untrimmed[index] ? false :
			matches_untrimmed(untrimmed, s, index + 1);
	}
}




#define ENUM_CLASS(enumName, underlying, ...)											\
struct data_##enumName																	\
{																						\
	using underlying_t = underlying;													\
	enum { __VA_ARGS__ };																\
	static constexpr auto size = IDENTITY(COUNT(__VA_ARGS__));							\
																						\
	static constexpr std::array<underlying_t, size> values =							\
	{ IDENTITY(IGNORE_ASSIGN(__VA_ARGS__)) };											\
																						\
	static constexpr std::array<std::string_view, size> raw_names =						\
	{ IDENTITY(STRINGIZE(__VA_ARGS__)) };												\
};																						\
																						\
class enumName																			\
{																						\
	using underlying_t = underlying;													\
																						\
public:																					\
	enum enum_t : underlying_t { __VA_ARGS__ };											\
																						\
	constexpr enumName(const enum_t value)												\
		: value(value)																	\
	{}																					\
																						\
	constexpr operator enum_t() const													\
	{																					\
		return static_cast<enum_t>(value);												\
	}																					\
																						\
	[[nodiscard]] constexpr enum_t ToEnum() const										\
	{																					\
		return static_cast<enum_t>(value);												\
	}																					\
																						\
	[[nodiscard]] constexpr underlying_t ToUnderlying() const							\
	{																					\
		return value;																	\
	}																					\
																						\
	[[nodiscard]] std::string_view ToString() const										\
	{																					\
		const auto name = ToStringImpl(static_cast<enum_t>(value));						\
		return name;																	\
	}																					\
																						\
	static constexpr enum_t FromString(const std::string_view& s, size_t index = 0)		\
	{																					\
		using namespace klib::kEnum::secret::helper;									\
																						\
		if (index >= data_##enumName::size)												\
			std::_Xout_of_range("Invalid identifier");									\
																						\
		const auto matches = matches_untrimmed(data_##enumName::raw_names[index], s);	\
																						\
		const auto ret = matches														\
			? static_cast<enum_t>(data_##enumName::values[index])						\
			: FromString(s, index + 1);													\
																						\
		return ret;																		\
	}																					\
																						\
	[[nodiscard]]																		\
	static constexpr std::string_view PrettyType()										\
	{																					\
		return "enum " #enumName;														\
	}																					\
																						\
	[[nodiscard]]																		\
	static std::string PrettyType(const enumName input)									\
	{																					\
		const auto type = std::string(PrettyType());									\
		const auto name = std::string(ToStringImpl(input));								\
		return type + "::" + name;														\
	}																					\
																						\
	private:																			\
	[[nodiscard]] static std::string_view ToStringImpl(enumName input)					\
	{																					\
		const auto name = TrimmedNames(static_cast<enum_t>(input));						\
		return name;																	\
	}																					\
																						\
		static std::string_view TrimmedNames(const enumName input)						\
	{																					\
		static std::unique_ptr<char[]> the_names[data_##enumName::size];				\
		static bool  initialized = false;												\
		size_t index = 0;																\
																						\
		if (!initialized)																\
		{																				\
			InitializeNames(the_names);													\
			initialized = true;															\
		}																				\
																						\
		do																				\
		{																				\
			if (input == data_##enumName::values[index])								\
				break;																	\
		}																				\
		while (++index < data_##enumName::size);										\
																						\
		return the_names[index].get();													\
	}																					\
																						\
	static constexpr void InitializeNames(std::unique_ptr<char[]>						\
		(&the_names)[data_##enumName::size])											\
	{																					\
		using namespace klib::kEnum::secret::helper;									\
		for (auto i = 0; i < data_##enumName::size; ++i)								\
		{																				\
			const auto & raw_name = data_##enumName::raw_names[i];						\
			auto& name = the_names[i];													\
																						\
			const auto length_til_terminator =											\
				std::strcspn(raw_name.data(),											\
					terminators);														\
																						\
			name.reset(new char[length_til_terminator + 1]);							\
																						\
			strncpy_s(name.get(),														\
				length_til_terminator + 1,												\
				raw_name.data(),														\
				length_til_terminator);													\
																						\
			name[length_til_terminator] = '\0';											\
		}																				\
	}																					\
																						\
private:																				\
	underlying_t value;																	\
};																						\
