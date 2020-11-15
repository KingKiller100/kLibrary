#pragma once

#include "../../../HelperMacros.hpp"
#include "../../../TypeTraits/StringTraits.hpp"
#include "../../../TypeTraits/CustomTraits.hpp"

#include "../kStringTypes.hpp"

#include <type_traits>


namespace klib::kString::stringify
{
	template<typename Char_t, typename T, typename Enabled_t = void>
	class Identity { constexpr Identity() = delete; };

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/// User made Custom types - Must have a function ToString that returns an object with a 
	/// "data()" function which returns a const CharType pointer
	/////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<type_trait::Is_CustomType_V<T>>>
	{
	public:
		constexpr Identity(const T& obj)
			: data(Convert<Char_t>(obj.ToString()))
		{}
		USE_RESULT constexpr decltype(auto) Get() const
		{
			return data.data();
		}
		USE_RESULT constexpr decltype(auto) GetPtr()
		{
			return std::addressof(data);
		}

	private:
		const StringWriter<Char_t>& data;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// C++ STL string/string_view
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& type_trait::Is_StringType_V<T>
		&& std::is_same_v<typename T::value_type, Char_t>
		>>
	{
	public:
		constexpr Identity(const T& str)
			: data(str)
		{}
		USE_RESULT constexpr auto Get() const
		{
			return data.data();
		}
		USE_RESULT constexpr auto GetPtr() const
		{
			return std::addressof(data);
		}

	private:
		const T& data;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	/// Character Pointers
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& type_trait::Is_CharType_V<ONLY_TYPE(T)>
		&& std::is_same_v<Char_t, ONLY_TYPE(T)>
		&& std::is_pointer_v<T>
		>>
	{
	public:
		constexpr Identity(const T& str)
			: data(str)
		{}
		USE_RESULT constexpr auto Get() const
		{
			return data;
		}
		USE_RESULT constexpr auto GetPtr() const
		{
			return std::addressof(data);
		}

	private:
		const T& data;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////////
	///// Mon-Character Pointers
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& !type_trait::Is_CharType_V<ONLY_TYPE(T)>
		&& std::is_pointer_v<T>
		>>
	{
	public:
		constexpr Identity(const T& str)
			: data(str)
		{}
		USE_RESULT constexpr auto Get() const
		{
			return data;
		}
		USE_RESULT constexpr auto GetPtr() const
		{
			return reinterpret_cast<const void* const*>(std::addressof(data));
		}

	private:
		const T& data;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////
	///// Primitive types (char, int, double, unsigned long long,...)
	///////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Char_t, typename T>
	class Identity<Char_t, T, std::enable_if_t<
		type_trait::Is_CharType_V<Char_t>
		&& std::is_arithmetic_v<T>
		>>
	{
	public:
		constexpr Identity(const T& str)
			: data(str)
		{}
		USE_RESULT constexpr decltype(auto) Get() const
		{
			return data;
		}
		USE_RESULT constexpr decltype(auto) GetPtr() const
		{
			return std::addressof(data);
		}

	private:
		const T& data;
	};

	
	///////////////////////////////////////////////////////////////////////////////////////////////////
	///// Primitive array types (char, int, double, unsigned long long,...)
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//template<typename CharType, typename T, size_t Size>
	//constexpr
	//	std::enable_if_t <
	//	type_trait::Is_CharType_V<CharType>
	//	&& std::is_arithmetic_v<T>
	//	&& !std::is_pointer_v<T>
	//	&& std::is_array_v<T[Size]>
	//	, const T*>
	//	Identity(const T(&obj)[Size])
	//{
	//	return std::addressof(obj[0]);
	//}

	//template<typename CharType, typename T, size_t Size>
	//constexpr
	//	std::enable_if_t <
	//	type_trait::Is_CharType_V<CharType>
	//	&& std::is_arithmetic_v<T>
	//	&& !std::is_pointer_v<T>
	//	&& std::is_array_v<T[]>
	//	, const T* const*>
	//	IdentityPtr(const T(&obj)[Size])
	//{
	//	auto ptr = Identity<CharType, T, Size>(obj);
	//	return std::addressof(ptr);
	//}

}
