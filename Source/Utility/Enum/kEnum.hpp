#pragma once

#include "kEnumCore.hpp"
#include "../String/kStringConverter.hpp"
#include <functional>

#define ENUM_X(x, enumName, underlying, ...)											\
x enumName																				\
{																						\
public:																					\
	using underlying_t = underlying;													\
	enum InternalEnum_t : underlying { __VA_ARGS__ };									\
																						\
protected:																				\
	struct secret_impl_##enumName														\
	{																					\
		static constexpr auto size = IDENTITY(COUNT(__VA_ARGS__));						\
																						\
		static constexpr std::array<underlying, size> values =							\
		{ IDENTITY(IGNORE_ASSIGN(__VA_ARGS__)) };										\
																						\
		static constexpr std::array<std::string_view, size> raw_names =					\
		{ IDENTITY(STRINGIZE(__VA_ARGS__)) };											\
	};																					\
																						\
public:																					\
	constexpr enumName(InternalEnum_t value)											\
		: value(value)																	\
	{}																					\
																						\
	constexpr enumName(underlying val)													\
		: value(val)																	\
	{																					\
		const auto& v = secret_impl_##enumName::values;									\
		if (std::find(std::begin(v), std::end(v), val) == v.end())						\
		{																				\
			throw std::out_of_range("Value given is out of enum's range");				\
		}																				\
	}																					\
																						\
	constexpr enumName& operator=(const InternalEnum_t value)							\
	{																					\
		this->value = value;															\
		return *this;																	\
	}																					\
																						\
	constexpr operator InternalEnum_t() const											\
	{																					\
		return static_cast<InternalEnum_t>(value);										\
	}																					\
																						\
	USE_RESULT constexpr InternalEnum_t ToEnum() const									\
	{																					\
		return static_cast<InternalEnum_t>(value);										\
	}																					\
																						\
	USE_RESULT constexpr underlying ToUnderlying() const								\
	{																					\
		return value;																	\
	}																					\
																						\
	USE_RESULT static constexpr size_t Count() noexcept									\
	{																					\
		return secret_impl_##enumName::size;											\
	}																					\
																						\
	USE_RESULT constexpr bool MaskCmp(InternalEnum_t target) const						\
	{																					\
		return MaskCmp(target, true, false);											\
	}																					\
																						\
	template<typename T1, typename T2, typename = std::enable_if_t<						\
	std::is_convertible_v<T2, T1>														\
	>>																					\
	USE_RESULT constexpr std::decay_t<T1> MaskCmp(InternalEnum_t mask					\
		, T1&& successState, T2&& failState) const										\
	{																					\
		if (mask & value)																\
			return successState;														\
		return failState;																\
	}																					\
																						\
	USE_RESULT constexpr bool Compare(InternalEnum_t target) const						\
	{																					\
		return Compare(target, true, false);											\
	}																					\
																						\
	template<typename T1, typename T2>													\
	USE_RESULT constexpr std::decay_t<T1> Compare(InternalEnum_t target					\
		, T1&& successState, T2&& failState) const										\
	{																					\
		if (target == value)															\
			return successState;														\
		return failState;																\
	}																					\
																						\
	static constexpr enumName FromString(const std::string_view& search)				\
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
	USE_RESULT const Char_t* ToString() const											\
	{																					\
		const auto name = ToStringImpl<Char_t>();										\
		return name;																	\
	}																					\
																						\
	USE_RESULT constexpr bool operator<(const enumName& other) const 					\
	{																					\
		return value < other.value;														\
	}																					\
																						\
	USE_RESULT constexpr bool operator<=(const enumName& other) const 					\
	{																					\
		return value <= other.value;													\
	}																					\
																						\
	template<typename T>																\
	USE_RESULT constexpr bool operator<(const T& other) const 							\
	{																					\
		return value < other;															\
	}																					\
																						\
	template<typename T>																\
	USE_RESULT constexpr bool operator<=(const T& other) const 							\
	{																					\
		return value <= other;															\
	}																					\
																						\
	static void ForEach(std::function<void(enumName)> func)								\
	{																					\
		for (auto v : secret_impl_##enumName::values)									\
		{																				\
			func(v);																	\
		}																				\
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
	static constexpr InternalEnum_t FromStringImpl(const char* s, size_t index)			\
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
			? static_cast<InternalEnum_t>(secret_impl_##enumName::values[index])		\
			: FromStringImpl(s, index + 1);												\
																						\
		return ret;																		\
	}																					\
																						\
private:																				\
	underlying value;																	\
};																						\

#define ENUM_X_FWD_DCL(x, enumName) x enumName

#define ENUM_CLASS_FWD_DCL(enumName) ENUM_X_FWD_DCL(class, enumName)
#define ENUM_CLASS(enumName, underlying, ...) ENUM_X(class, enumName, underlying, __VA_ARGS__)

#define ENUM_STRUCT_FWD_DCL(enumName) ENUM_X_FWD_DCL(struct, enumName)
#define ENUM_STRUCT(enumName, underlying, ...) ENUM_X(struct, enumName, underlying, __VA_ARGS__)

