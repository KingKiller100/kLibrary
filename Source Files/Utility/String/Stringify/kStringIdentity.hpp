#pragma once

#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../../TypeTraits/CustomTraits.hpp"

#include <type_traits>
#include <vector>


namespace klib::kString::stringify
{
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Non-primitive custom types - Must have a function ToString that returns an object with a 
	/// "data()" function which returns a const CharType pointer
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//template<typename Char_t, typename T, bool IsCustom>
	//struct IdentityBase { IdentityBase() = delete; };

	//template<typename Char_t, typename T>
	//struct IdentityBase<T, Char_t, type_trait::Is_Custom_V<T>>
	//{
	//	constexpr IdentityBase() {}
	//};

	//template<typename Char_t, typename T>
	//struct Identity : public IdentityBase<Char_t, T, true>
	//{
	//	constexpr Identity() = default;

	//	
	//};
	
	template<typename CharType, typename T>
	constexpr const std::basic_string<CharType>& GetObjectString(const T& obj)
	{
		static std::vector<std::basic_string<CharType>> storage =
			decltype(storage)();

		const auto string = Convert<CharType>(obj.ToString());

		static_assert(std::is_same_v<ONLY_TYPE(decltype(string)), std::basic_string<CharType>>
			, "Custom class's ToString func result type must be a string type that contains the"
			" same character types as CharType i.e. std::string -> char, std::u16string -> char16_t, etc");

		auto iter = std::find(storage.begin(), storage.end(), string);
		if (iter != storage.end())
			return (*iter);

		const std::basic_string<CharType>& value = storage.emplace_back(string);
		return value;
	}

	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_Custom_V<T>
		, const CharType*
		>
		Identity(const T& obj)
	{
		const std::basic_string<CharType>& string = GetObjectString<CharType>(obj);
		return string.data();
	}

	template<typename CharType, typename T>
	constexpr
		std::enable_if_t <
		type_trait::Is_Custom_V<T>
		, const std::basic_string<CharType>*>
		IdentityPtr(const T& obj)
	{
		const std::basic_string<CharType>& string = GetObjectString<CharType>(obj);
		return std::addressof(string);
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// C++ STL string/string_view
	/////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_CharType_V<CharType>
		&& type_trait::Is_StringType_V<T>
		, const typename T::value_type*
		>
		Identity(const T& str)
	{
		static_assert(std::is_same_v<CharType, typename T::value_type>, 
			"Must be a string type that contains the same character types as CharType "
			"i.e. std::string -> char, std::u16string -> char16_t");
		return str.data();
	}

	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_CharType_V<CharType>
		&& type_trait::Is_StringType_V<T>
		, const T*
		>
		IdentityPtr(const T& str)
	{
		static_assert(std::is_same_v<CharType, typename T::value_type>,
			"Must be a string type that contains the same character types as CharType "
			"i.e. std::string -> char, std::u16string -> char16_t");
		return std::addressof(str);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Pointers
	/////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_CharType_V<CharType>
		&& std::is_pointer_v<T>
		, const T
		>
		Identity(const T obj)
	{
		return obj;
	}

	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_CharType_V<CharType>
		&& type_trait::Is_CharType_V<ONLY_TYPE(T)>
		&& std::is_same_v<CharType, ONLY_TYPE(T)>
		&& std::is_pointer_v<T>
		, const T*
		>
		IdentityPtr(const T& obj)
	{
		return std::addressof(obj);
	}

	// Cast non-characters pointer as void pointer
	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_CharType_V<CharType>
		&& !type_trait::Is_CharType_V<ONLY_TYPE(T)>
		&& std::is_pointer_v<T>
		, const void* const*
		>
		IdentityPtr(const T& obj)
	{
		return (const void* const*)std::addressof(obj);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Primitive types (char, int, double, unsigned long long,...)
	/////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename CharType, typename T>
	constexpr
		std::enable_if_t<
		type_trait::Is_CharType_V<CharType>
		&& std::is_arithmetic_v<T>
		, T
		>
		Identity(T obj)
	{
		return obj;
	}


	template<typename CharType, typename T>
	constexpr
		std::enable_if_t <
		type_trait::Is_CharType_V<CharType>
		&& std::is_arithmetic_v<T>
		, const T*>
		IdentityPtr(const T& obj)
	{
		return std::addressof(obj);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// Primitive array types (char, int, double, unsigned long long,...)
	/////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename CharType, typename T, size_t Size>
	constexpr
		std::enable_if_t <
		type_trait::Is_CharType_V<CharType>
		&& std::is_arithmetic_v<T>
		&& !std::is_pointer_v<T>
		&& std::is_array_v<T[Size]>
		, const T*>
		Identity(const T(&obj)[Size])
	{
		return std::addressof(obj[0]);
	}

	template<typename CharType, typename T, size_t Size>
	constexpr
		std::enable_if_t <
		type_trait::Is_CharType_V<CharType>
		&& std::is_arithmetic_v<T>
		&& !std::is_pointer_v<T>
		&& std::is_array_v<T[]>
		, const T* const*>
		IdentityPtr(const T(&obj)[Size])
	{
		auto ptr = Identity<CharType, T, Size>(obj);
		return std::addressof(ptr);
	}

}
